#ifndef PEER_H
#define PEER_H

typedef struct p {
    char* id;
    char* ip;
    int port;
} Peer;

void add_peer(Peer *p);
Peer* get_peer(char *id);
Peer** get_all_peers();

#endif