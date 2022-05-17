#include "peer.h"
#include "file.h"
#ifndef NETWORK_H
#define NETWORK_H

#define SIZE 1024

void send_file(FILE *fp, int sockfd);
void write_file(int sockfd, char *filename);
void tcp_send_file(Peer *p, char *filename);


#endif