#ifndef RPC_H
#define RPC_H

typedef struct node {
    int type;
    char filename[256];
    int nbLinks;
    char links[MAX_LINKS][CID_LENGTH + 1];
    unsigned char data[BLOCK_LIMIT];
    int size;
} DAGNode;

#endif