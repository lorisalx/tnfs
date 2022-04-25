#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "file.h"
#include "tnfs.h"
#include "logger.h"
#include "dag.h"
#include "redis.h"
#include "sha-256.h"

char** create_chunks(char* data, size_t data_size, int nb_blocks)
{
    // Create CID links array
    char **linksTab = calloc(nb_blocks, sizeof(char *));
    for(int i = 0; i < nb_blocks; i++) {
        linksTab[i] = calloc(1, sizeof(char) * CID_LENGTH);
    }

    // Divide the data into blocks
    for(int i = 0; i < nb_blocks; i++) {
        char* currentBloc = (char *) calloc(BLOCK_LIMIT, sizeof(char));

        int currentBlockSize = 0;

        for(int j = 0; j < BLOCK_LIMIT; j++) {
            if(data_size == 0) 
                break;
            
            currentBloc[j] = data[(i*BLOCK_LIMIT) + j];
            data_size--;
            currentBlockSize++;
        }

        // Block creation
        DAGNode* node = calloc(1, sizeof(DAGNode));
        node->type = 1;
        node->nbLinks = 0;
        node->size = currentBlockSize;
        strcpy(node->filename, "null");
        memcpy(node->data, currentBloc, BLOCK_LIMIT);

        // Block name
        char blockname[NAME_MAX], filedir[PATH_MAX];
        char *hash_string = (char *)malloc(sizeof(char) * HASH_LENGTH);
        generate_block_name(currentBloc, currentBlockSize, hash_string);
        snprintf(blockname, sizeof(blockname), "%s.dat", hash_string);
        snprintf(filedir, sizeof(filedir), "%s%s", BLOCK_PATH, blockname);

        // Write
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
        free(node);
        free(hash_string);
    }

    return linksTab;
}

void create_parent(char* filename, char** links, int nb_links, char* cid, size_t content_size) {
    // Block creation
    DAGNode* node = calloc(1, sizeof(DAGNode));
    node->type = 2;
    node->nbLinks = nb_links;
    node->size = content_size;
    strcpy(node->filename, filename);

    for(int i = 0; i < MAX_LINKS; i++) {
        if(i >= nb_links) {
            strcpy(node->links[i], "");
            break;
        } else {
            strcpy(node->links[i], links[i]);
        }
    }

    // Concat a parent block into a string (filename + all CIDs) => to have a block name
    int result_size = nb_links * CID_LENGTH + NAME_MAX;
    char* result = calloc(1, result_size * sizeof(char));
    concat_parents_to_string(filename, links, nb_links, result);

    // Block name
    char *hash_string = (char *) malloc(sizeof(char) * HASH_LENGTH);
    char blockname[NAME_MAX], filedir[PATH_MAX];
    generate_block_name(result, result_size, hash_string);
    snprintf(blockname, sizeof(blockname), "%s.dat", hash_string);
    snprintf(filedir, sizeof(filedir), "%s%s", BLOCK_PATH, blockname);

    // Write
    FILE *output_parent;
    output_parent = fopen(filedir, "wb");
    fwrite (node, sizeof(DAGNode), 1, output_parent);
    fclose(output_parent);

    // Generate CID of the block
    generate_cid_from_file(filedir, cid);

    // Adding the tuple (cid=>block_name) into redis
    set_redis_command(cid, blockname);

    free(node);
    free(result);
    free(hash_string);
}

int get_number_of_blocks(size_t data_size) {
    return (int) (data_size / BLOCK_LIMIT) + 1;
}

int verify_block(char* filename, char* cid) {
    char* block_cid = (char*) calloc(1, sizeof(char) * CID_LENGTH);
    generate_cid_from_file(filename, block_cid);
    return (strcmp(block_cid,cid) == 0) ? 1 : -1;
}

void read_block(char* cid, DAGNode* node) {  
    char block_name[NAME_MAX];
    get_redis_command(cid, block_name);

    if(strcmp(block_name,"null") == 0) {
        log_error("You don't have the block.");
        // Download du block ici
        exit(EXIT_FAILURE);
    }

    char buf[PATH_MAX];
    snprintf(buf, sizeof(buf), "%s%s", BLOCK_PATH, block_name);

    // Verify block
    if(verify_block(buf, cid) == -1) {
        log_error("The block doesn't correspond to the CID.");
        exit(EXIT_FAILURE);
    }

    // Read block
    FILE *f;

    if ((f = fopen(buf, "rb")) == NULL){
        log_error("An error occured while loading the block.");
        exit(EXIT_FAILURE);
    }

    fread(node, sizeof(DAGNode), 1, f);
    fclose (f);
}

void concat_parents_to_string(char* filename, char** array, int size, char* result) {
    strcat(result, filename);

    for(int i = 0; i < size; i++) {
        strcat(result, array[i]);
    }
}