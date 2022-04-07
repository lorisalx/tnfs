#include "tnfs.h"

#ifndef DAG_H
#define DAG_H

char** create_chunks(char* data, size_t data_size, int nb_blocks); // Create blocks from all the content of the file
void create_parent(char* filename, char** links, int nb_links, char* cid); // Create a parent node
int get_number_of_blocks(size_t data_size); // Get number of block needed to split one file

struct BlockNode {
    int type;
    unsigned char bytes[BLOCK_LIMIT];
};

typedef struct pn {
    int type;
    char filename[256];
    int nbBlocks;
    char* links[MAX_LINKS];
} ParentNode;

#endif //DAG_H
