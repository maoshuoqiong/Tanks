#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h> /* for bzero */
#include <string.h> /* for strlen */

#define TANK_PORT 10127
typedef struct sockaddr SA;

#ifndef MAX_LINE
#define MAX_LINE (4096)
#endif

static int open_max = FD_SETSIZE;
static int fds[FD_SETSIZE];

static void tell_all(int fd, const char buf[], int len)
{
	for(int i=1;i<open_max;i++)
		if(fds[i]!=-1)
			if(write(fds[i], buf, len)!=len)
				perror("write error");
}

int main(int argc, const char * argv[])
{
/*
	long open_max = sysconf(_SC_OPEN_MAX);
	if(open_max<0)
		perror("sysconf error"),exit(1);
*/
	printf("Open_max: %d.\n",open_max);

	int maxid;
	int sockfd, clientfd;
	socklen_t clilen;
	struct sockaddr_in server, client; 
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		perror("socket error"),exit(1);	

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port   = htons(TANK_PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (SA*)&server, sizeof(server))<0)
		perror("bind error"),exit(1);

	if(listen(sockfd,5)<0)
		perror("listen error"),exit(1);

	/* init fds */
	for(int i=1;i<open_max;i++)
		fds[i] = -1;

	maxid = sockfd;

	fd_set readfds, allfds;
	FD_ZERO(&allfds);
	FD_SET(sockfd,&allfds);
	fds[0] = sockfd;

	char buf[MAX_LINE];
	int n;

	while(1)
	{
		readfds = allfds;
		if((n=select(maxid+1,&readfds,NULL,NULL,NULL))<0)
			perror("select error"),exit(1);

		if(FD_ISSET(sockfd,&readfds))
		{
			clilen = sizeof(client);
			if((clientfd=accept(sockfd, (SA*)&client,	&clilen))<0)
			{
				perror("accept error");
				continue;
			}

			if(clientfd>maxid)
				maxid = clientfd;

			int i;
			for(i=1;i<open_max;i++)
			{
				if(fds[i]==-1)
				{
					fds[i] = clientfd;
					break;
				}
			}
			
			if(i>=open_max)
			{
				sprintf(buf,"To many connections.\n");
				write(clientfd, buf, strlen(buf));
				close(clientfd);
				continue;	
			}

			FD_SET(clientfd,&allfds);

			sprintf(buf,"%s:%d connected.\n",
					inet_ntoa(client.sin_addr),
					ntohs(client.sin_port)); 	

			tell_all(sockfd, buf, strlen(buf));

			if(--n==0)
				continue;
				
		}		

		for(int i=1; i<open_max;i++)
		{
			if(FD_ISSET(fds[i],&readfds))
			{
				if((n=read(fds[i],buf,sizeof(buf)))>0)
				{
					printf("size: %d.\n",n);
					tell_all(fds[i], buf, n);
				}
				else if(n==0)
				{
					printf("client is closed.\n");
					FD_CLR(fds[i],&allfds);	
					close(fds[i]);
					fds[i] = -1;
				}
				else
				{	
					FD_CLR(fds[i],&allfds);	
					perror("read error");
					close(fds[i]);
					fds[i] = -1;
				}

				if(--n==0)
					continue;	
			}
		}

	}

	exit(0);
}

