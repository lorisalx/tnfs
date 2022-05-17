#include "peer.h"
#include "file.h"
#ifndef NETWORK_H
#define NETWORK_H

#define SIZE 1024

void send_file(FILE *fp, int sockfd);
void write_file(int sockfd, char *filename);
void tcp_send_file(Peer *p, char *filename);
void look_for_block(char* cid);
int open_tcp_socket();
void close_tcp_socket(int sockfd);
void send_txt_tcp(int sockfd, char* txt);
void send_comm_type();

#endif