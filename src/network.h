#include "peer.h"
#include "file.h"
#ifndef NETWORK_H
#define NETWORK_H

typedef enum { 
    UDP_CONNECTION, 
    TCP_CONNECTION
} CONNECTION_TYPE;

#define SIZE 1024

//void send_file(FILE *fp, int sockfd);
//void write_file(int sockfd, char *filename);
//void tcp_send_file(Peer *p, char *filename);
void look_for_block(char* cid);
int create_socket(CONNECTION_TYPE type, struct sockaddr_in* servaddr);
void close_socket(int sockfd);

// void send_txt_tcp(int sockfd, char* txt);

#endif