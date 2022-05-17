#include "file.h"
#include "tnfs.h"
#include "logger.h"
#include "dag.h"
#include "redis.h"
#include "string.h"

#include "network.h"
#include "peer.h"

#include <stdio.h>

void add_tnfs_file(char* filename)
{
    struct stat *buffer = malloc(sizeof(struct stat));
    get_file_stats(filename, buffer);

    if(buffer->st_size == 0)
    {
        log_error("The given file is empty.");
        return;
    }

    if(buffer->st_size > MAX_FILE_LENGTH)
    {
        log_formated_error("The given file is too big (max: %ld bytes).", MAX_FILE_LENGTH);
        return;
    }

    // Get file data
    char* output = (char *) malloc(buffer->st_size * sizeof(char));
    size_t output_length = get_file_content(filename, output);

    // Create blocks from the file
    int nbBlocks = get_number_of_blocks(output_length);
    char** linksTab = create_chunks(output, output_length, nbBlocks);

    // Generate the parent bloc and get the CID
    char* cid = (char *) calloc(1, sizeof(char) * CID_LENGTH);
    create_parent(filename, linksTab, nbBlocks, cid, output_length);
    log_formated_info("New file %s added to tnfs with CID : %s", filename, cid);

    free(output);
    free(buffer);
    free(linksTab);
    free(cid);
}

void tnfs_get_infos(char* cid) {
    DAGNode* node = calloc(1, sizeof(DAGNode));
    read_block(cid, node);

    // Print infos
    printf("-- %s --\n",cid);
    printf("Type : %s\n", (node->type == 1) ? "Block" : "File");
    if(node->type == 1) {
        printf("Data Size : %i\n", node->size);
    } else {
        printf("Filename : %s\n", node->filename);
        printf("File Size : %d kb(s)\n", (node->size/1000));
        printf("Amount of blocks : %i\n", node->nbLinks);

        for(int i = 0; i < node->nbLinks; i++) {
            printf("    - %s\n", node->links[i]);
        }
    }

    free(node);
}

void get_tnfs_file(char* cid) {
    DAGNode* node = calloc(1, sizeof(DAGNode));
    read_block(cid, node);

    if(node->type != 2) {
        log_error("This CID is a block, not a file.");
        return;
    }

    // File creation
    remove(node->filename);

    FILE *output;
    output = fopen(node->filename, "ab+");
    for(int i = 0; i < node->nbLinks; i++) {
        DAGNode* currentBlock = calloc(1, sizeof(DAGNode));
        read_block(node->links[i], currentBlock);
        fwrite(currentBlock->data, sizeof(char) * currentBlock->size, 1, output);
        free(currentBlock);
    }

    fclose(output);
    free(node);
}

void tnfs_clean_data() {
    clean_registry(BLOCK);
    // To do : clean blocks folder
}

void tnfs_test() {
    Peer *p = calloc(1,sizeof(Peer));
    p->ip = "127.0.0.1";
    p->port = 5000;
    add_peer(p);
    Peer *pget = calloc(1,sizeof(Peer));
    get_peer("customid", pget);
    Peer* ptab[MAX_REDIS_KEYS];
    *ptab = calloc(MAX_REDIS_KEYS,sizeof(Peer));
    get_all_peers(ptab);
    //tcp_send_file(p,"caca");
}