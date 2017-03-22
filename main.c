#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>

#define TANK_PORT 10127
typedef struct sockaddr SA;

int main(int argc, const char * argv[])
{
	int sockfd, clientfd;
	socklen_t clilen;
	struct sockaddr_in server, client; 
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		perror("socket error"),exit(1);	

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port   = htons(TANK_PORT);
	server.sin_addr.s_addr = htons(INADDR_ANY);

	if(bind(sockfd, (SA*)&server, sizeof(server))<0)
		perror("bind error"),exit(1);

	if(listen(sockfd,5)<0)
		perror("listen error"),exit(1);

	int n;
	fd_set readfds;
	FD_ZERO(&readfds);

	while(1)
	{
		FD_SET(sockfd,&readfds);
		if((n=select(sockfd+1,&readfds,NULL,NULL,NULL))<0)
			perror("select error"),exit(1);

		printf("Amount: %d.\n",n);
		if(FD_ISSET(sockfd,&readfds))
		{
			clilen = sizeof(client);
			if((clientfd=accept(sockfd, (SA*)&client,	&clilen))<0)
			{
				perror("accept error");
				continue;
			}

			printf("Client %s:%d connect.\n",
					inet_ntoa(client.sin_addr),
					ntohs(client.sin_port)); 	
			close(clientfd);
		}		
	}
	exit(0);
}

