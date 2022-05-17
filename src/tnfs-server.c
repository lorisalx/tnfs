#include "tnfs-server.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "logger.h"
#include "network.h"

int max(int x, int y)
{
	if (x > y)
		return x;
	else
		return y;
}
int main()
{
	int tcpfd, connfd, udpfd, nready, maxfdp1;
	char buffer[MAXLINE];
	pid_t childpid;
	fd_set rset;
	ssize_t n;
	socklen_t len;
	const int on = 1;
	struct sockaddr_in cliaddr, servaddr;
	char* message = "Hello Client";
	void sig_chld(int);

	/* Opening TCP socket */
    tcpfd = socket(AF_INET, SOCK_STREAM, 0);
	if(tcpfd < 0) {
        log_error("Creation of listening TCP socket failed.");
        exit(EXIT_FAILURE);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	/* Binding TCP socket */
	if(bind(tcpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        log_error("Binding of listening TCP socket failed.");
        exit(EXIT_FAILURE);
	}
	listen(tcpfd, 10);
    log_formated_info("Connected on port %d",PORT);

	/* Opening UDP socket */
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(udpfd < 0 ) {
     	log_error("Creation of listening UDP socket failed.");
    	exit(EXIT_FAILURE);
	}

	if(bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
     	log_error("Binding of listening UDP socket failed.");
    	exit(EXIT_FAILURE);
	}

	/* Clearing the descriptor set */
	FD_ZERO(&rset);


	maxfdp1 = max(tcpfd, udpfd) + 1;
	for (;;) {

		/* Set both fd in readset */
		FD_SET(tcpfd, &rset);
		FD_SET(udpfd, &rset);

		/* Select the ready descriptor */
		nready = select(maxfdp1, &rset, NULL, NULL, NULL);

		// if tcp socket is readable then handle
		// it by accepting the connection
		if (FD_ISSET(tcpfd, &rset)) {
			len = sizeof(cliaddr);
			connfd = accept(tcpfd, (struct sockaddr*)&cliaddr, &len);
			if ((childpid = fork()) == 0) {
				close(tcpfd);
                write_file(connfd,"filerecieve");
				close(connfd);
				exit(0);
			}
			close(connfd);
		}
		// if udp socket is readable receive the message.
		if (FD_ISSET(udpfd, &rset)) {
			len = sizeof(cliaddr);
			bzero(buffer, sizeof(buffer));
			printf("\nMessage from UDP client: ");
			n = recvfrom(udpfd, buffer, sizeof(buffer), 0,
						(struct sockaddr*)&cliaddr, &len);
			puts(buffer);
			sendto(udpfd, (const char*)message, sizeof(buffer), 0,
				(struct sockaddr*)&cliaddr, sizeof(cliaddr));
		}
	}
}
