#include "file.h"
#include "tnfs.h"
#include "logger.h"
#include "dag.h"
#include "redis.h"
#include "string.h"

#include <stdio.h>

void add_tnfs_file(char* filename)
{
    struct stat *buffer = malloc(sizeof(struct stat));
    int status = get_file_stats(filename, buffer);

    if(status != 0)
    {
        log_formated_error("The given file (%s) is not found.", filename);
        return;
    }

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
    if(output_length == 0)
    {
        log_error("An error occured while reading the file.");
        return;
    }

    // Create blocks from the file
    int nbBlocks = get_number_of_blocks(output_length);
    char** linksTab = create_chunks(output, output_length, nbBlocks);

    // Generate the parent bloc and get the CID
    char* cid = (char *) calloc(1, sizeof(char) * CID_LENGTH);
    create_parent(filename, linksTab, nbBlocks, cid);
    log_formated_info("New file %s added to tnfs with CID : %s", filename, cid);

    free(output);
    free(buffer);
    free(linksTab);
    free(cid);
}

void tnfs_get_infos(char* cid) {
    DAGNode* node = calloc(1, sizeof(DAGNode));
    
    if(read_block(cid, node) == -1) {
        exit(EXIT_FAILURE);
    }

    // Print infos
    printf("- Block type : %i\n", node->type);
    printf("- Filename : %s\n", node->filename);
    printf("- Nb links : %i\n", node->nbLinks);

    free(node);
}

void get_tnfs_file(char* cid) {
    // Récupération du block parent
}