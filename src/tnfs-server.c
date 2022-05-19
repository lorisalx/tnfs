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
#include <string.h>

#include "logger.h"
#include "network.h"
#include "redis.h"

int max(int x, int y)
{
	if (x > y)
		return x;
	else
		return y;
}
int main()
{
	// Init redis
    if(init_redis() == -1) 
        return EXIT_FAILURE;
	int tcpfd, connfd, udpfd, nready, maxfdp1;
	char commtype[2];
	char buffer[500];
	char cid[CID_LENGTH];
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
				recv(connfd, commtype, sizeof(commtype), 0);

				log_formated_info("Communication type : %s",commtype);
				if (strcmp(commtype,"00") == 0) {
					char res[100];
					recv(connfd, cid, sizeof(cid), 0);
					log_formated_info("Looking for block : %s",cid);
					get_redis_command(BLOCK,cid,res);
					log_formated_info("Res %s",res);
					if(strcmp(res,"null") == 0) {
						log_error("Block not found.");
						send_txt_tcp(connfd,"0");
					}
					else {
						send_txt_tcp(connfd,"1");
						FILE *fp;
						fp = fopen("caca", "r+"); // File to open is res
						if (fp == NULL)
						{
							log_error("Error while reading file");
							exit(EXIT_FAILURE);
						}
						send_file(fp,connfd);
						log_info("File data sent successfully");
					}
				}
				else if (strcmp(commtype,"RECIEVE_FILE") == 0) {
					write_file(connfd,"filerecieve");
				}
				log_info("Closing comms ...");
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
