#include "peer.h"
#include "file.h"
#ifndef NETWORK_H
#define NETWORK_H

#define SIZE 1024

void look_for_block(char* cid);
int create_socket(Peer* p);
void close_socket(int sockfd);
int send_tcp(int sockfd, char* buffer);
void write_file(int sockfd, char *filename);

#endif