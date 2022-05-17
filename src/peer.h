#ifndef PEER_H
#define PEER_H

typedef struct p {
    char* id;
    char* ip;
    int port;
} Peer;

void add_peer(Peer *p);
void get_peer(char *id, Peer *p);
void get_all_peers(Peer* p[]);

#endif