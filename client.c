#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h> /* for bzero */

#define TANK_PORT 10127
typedef struct sockaddr SA;

#ifndef MAX_LINE
#define MAX_LINE (4096)
#endif

int main(int argc, const char* argv[])
{
	if(argc!=2)
		printf("usage %s <server>\n",argv[0]),exit(0);

	int sockfd;
	struct sockaddr_in server;

	if(( sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		perror("socket error"),exit(1);

	bzero(&server,sizeof(server));		
	server.sin_family = AF_INET;
	server.sin_port = htons(TANK_PORT);
	server.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sockfd,(SA*)&server,sizeof(server))<0)
		perror("connect error"),exit(1);

	char buf[MAX_LINE];
	int n, num;

	fd_set readfds, allfds;
	FD_ZERO(&allfds);
	FD_SET(STDIN_FILENO, &allfds);
	FD_SET(sockfd, &allfds);

	while(1)
	{
		readfds = allfds;
		if((num = select(sockfd+1, &readfds, NULL, NULL, NULL))<0)	/* return 0 when time out */
			perror("select error"),exit(1);
	
		if(FD_ISSET(STDIN_FILENO, &readfds))
		{
			if((n=read(STDIN_FILENO,buf,sizeof(buf))) > 0 )
			{
				if(write(sockfd,buf,n)!=n)
				{
					perror("write error");
					break;
				}
			}
			else if(n == 0)
				printf("EOF\n"),exit(0);
			else
				perror("read from stdin error"),exit(1);
		}

		if(FD_ISSET(sockfd, &readfds))
		{
			n = read(sockfd,buf,sizeof(buf));
			if(n == 0)
				printf("server is close\n"),exit(1);
			else if(n<0)
				perror("read error"),exit(1);
			else
			{
				buf[n] = '\n';
				write(STDOUT_FILENO,buf,n);
			}
		}
	}


	exit(0);
}

