#include "file.h"
#include "tnfs.h"
#include "logger.h"
#include "dag.h"
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
    }

    // Create the chunks from the file
    int nbBlocks = get_number_of_blocks(output_length);
    char** linksTab = create_chunks(output, output_length, nbBlocks);

    // Generate the parent bloc and get the CID
    char* cid = (char*)malloc(sizeof(char) * CID_LENGTH);
    create_parent(filename, linksTab, nbBlocks, cid);
    log_formated_info("New file %s added to tnfs with CID : %s", filename, cid);

    free(output);
    free(buffer);
    free(linksTab);
    free(cid);
}