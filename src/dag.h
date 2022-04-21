#include "tnfs.h"
#include "file.h"

#ifndef DAG_H
#define DAG_H

typedef struct dn {
    int type;
    char filename[256];
    int nbLinks;
    char links[MAX_LINKS][CID_LENGTH + 1];
    unsigned char data[BLOCK_LIMIT];
} DAGNode;

char** create_chunks(char* data, size_t data_size, int nb_blocks); // Create blocks from all the content of the file
void create_parent(char* filename, char** links, int nb_links, char* cid); // Create a parent node
int get_number_of_blocks(size_t data_size); // Get number of block needed to split one file
int verify_block(char* filename, char* cid); // Verify if filename == cid
int read_block(char* blockname, DAGNode* node); // Read the structure from a block

#endif //DAG_H
