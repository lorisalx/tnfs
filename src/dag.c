#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "file.h"
#include "tnfs.h"
#include "logger.h"
#include "dag.h"
#include "redis.h"

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
            if(data_size == 0) 
                break;
            
            currentBloc[j] = data[(i*BLOCK_LIMIT) + j];
            data_size--;
        }

        // Block creation
        DAGNode* node = calloc(1, sizeof(DAGNode));
        node->type = 1;
        node->nbLinks = 0;
        strcpy(node->filename, "null");
        memcpy(node->data, currentBloc, BLOCK_LIMIT);

        char blockname[50], filedir[256];
        snprintf(blockname, sizeof(blockname), "structure%i.dat", i);
        snprintf(filedir, sizeof(filedir), "%s%s", BLOCK_PATH, blockname);

        FILE *fou;
        fou = fopen(filedir, "wb");
        fwrite (node, sizeof(DAGNode), 1, fou);
        fclose(fou);

        // Generate CID of the block, and add it to the links array
        char* cid = (char*) calloc(1, sizeof(char) * CID_LENGTH);
        generate_cid_from_file(filedir, cid);
        linksTab[i] = cid;

        // Adding the tuple (cid=>block_name) into redis
        set_redis_command(cid, blockname);

        free(currentBloc);
    }

    return linksTab;
}

void create_parent(char* filename, char** links, int nb_links, char* cid) {
    DAGNode* node = calloc(1, sizeof(DAGNode));
    node->type = 2;
    node->nbLinks = nb_links;
    strcpy(node->filename, filename);

    for(int i = 0; i < MAX_LINKS; i++) {
        if(i >= nb_links) {
            strcpy(node->links[i], "null");
        } else {
            strcpy(node->links[i], links[i]);
        }
    }

    char blockname[50], filedir[256];
    snprintf(blockname, sizeof(blockname), "sparent.dat");
    snprintf(filedir, sizeof(filedir), "%s%s", BLOCK_PATH, blockname);

    FILE *output_parent;
    output_parent = fopen(filedir, "wb");
    fwrite (node, sizeof(DAGNode), 1, output_parent);
    fclose(output_parent);

    generate_cid_from_file(filedir, cid);

    // Adding the tuple (cid=>block_name) into redis
    set_redis_command(cid, blockname);

    free(node);
}

int get_number_of_blocks(size_t data_size) {
    return (int) (data_size / BLOCK_LIMIT) + 1;
}

int verify_block(char* filename, char* cid) {
    char* block_cid = (char*) calloc(1, sizeof(char) * CID_LENGTH);
    generate_cid_from_file(filename, block_cid);
    return (strcmp(block_cid,cid) == 0) ? 1 : -1;
}

int read_block(char* cid, DAGNode* node) {  
    char block_name[256];
    get_redis_command(cid, block_name);

    if(strcmp(block_name,"null") == 0) {
        log_error("You don't have the block.");
        return -1;
    }

    char buf[256];
    snprintf(buf, sizeof(buf), "%s%s", BLOCK_PATH, block_name);

    // Verify block
    if(verify_block(buf, cid) == -1) {
        log_error("The block doesn't correspond to the CID.");
        return -1;
    }

    // Read block
    FILE *f;
    f = fopen (buf, "rb");
    if (f == NULL) {
        log_error("An error occured while loading the block.");
        return -1;
    }

    fread(node, sizeof(DAGNode), 1, f);
    fclose (f);

    return 1;
}