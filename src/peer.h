#ifndef PEER_H
#define PEER_H

#define PORT_SIZE 5
#define IP_SIZE 30
#define MAX_PEER_SIZE IP_SIZE + PORT_SIZE + 2

typedef struct p {
    char* ip;
    int port;
} Peer;

void add_peer(Peer *p);
void remove_peer(Peer *p);
Peer** get_all_peers(int* results);
char* peer_to_string(Peer* p);

#endif