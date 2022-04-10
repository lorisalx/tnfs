#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "file.h"
#include "tnfs.h"
#include "logger.h"
#include "dag.h"

char** create_chunks(char* data, size_t data_size, int nb_blocks)
{
    // Create CID links array
    char **linksTab = calloc(nb_blocks, sizeof(char *));
    for(int i = 0; i < nb_blocks; i++) {
        linksTab[i] = calloc(1, sizeof(char) * CID_LENGTH);
    }

    for(int i = 0; i < nb_blocks; i++) {
        char* currentBloc = (char *) calloc(BLOCK_LIMIT, sizeof(char));

        for(int j = 0; j < BLOCK_LIMIT; j++) {
            if(j > data_size) 
                break;
            
            currentBloc[j] = data[(i*BLOCK_LIMIT) + j];
            data_size--;
        }

        // Block creation
        struct BlockNode blockNode = { 1 };
        memcpy(blockNode.bytes, currentBloc, BLOCK_LIMIT);

        char buf[256];
        snprintf(buf, sizeof(buf), "./blocks/structure%i.dat", i);

        FILE *fou;
        fou = fopen(buf, "wb");
        fwrite (&blockNode, sizeof(struct BlockNode), 1, fou);
        fclose(fou);

        // Generate CID of the block, and add it to the links array
        char* cid = (char*) calloc(1, sizeof(char) * CID_LENGTH);
        generate_cid_from_file(buf, cid);
        linksTab[i] = cid;

        free(currentBloc);
    }

    return linksTab;
}

void create_parent(char* filename, char** links, int nb_links, char* cid) {
    ParentNode* parentNode = calloc(1, sizeof(ParentNode));
    parentNode->nbBlocks = nb_links;
    parentNode->type = 2;
    strcpy(parentNode->filename, filename);

    for(int i = 0; i < MAX_LINKS; i++) {
        if(i >= nb_links) {
            strcpy(parentNode->links[i], "null");
        } else {
            strcpy(parentNode->links[i], links[i]);
        }
    }
    
    FILE *output_parent;
    output_parent = fopen("./parent.dat", "wb");
    fwrite (parentNode, sizeof(ParentNode), 1, output_parent);
    fclose(output_parent);

    generate_cid_from_file("./parent.dat", cid);
    free(parentNode);
}

int get_number_of_blocks(size_t data_size) {
    return (int) (data_size / BLOCK_LIMIT) + 1;
}