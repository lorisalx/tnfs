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
    int size;
} DAGNode;

char** create_chunks(char* data, size_t data_size, int nb_blocks); // Create blocks from all the content of the file
void create_parent(char* filename, char** links, int nb_links, char* cid, size_t content_size); // Create a parent node
int get_number_of_blocks(size_t data_size); // Get number of block needed to split one file
int verify_block(char* filename, char* cid); // Verify if the file == a cid
void read_block(char* blockname, DAGNode* node); // Read the structure from a block
void concat_parents_to_string(char* filename, char** array, int size, char* result); // Concat an array of string into a string

#endif //DAG_H
