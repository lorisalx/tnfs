#include "file.h"
#include "tnfs.h"
#include "logger.h"
#include "sha-256.h"
#include "base-32.h"
#include <stdio.h>

#define HASH_LENGTH SIZE_OF_SHA_256_HASH * 2
#define PLAIN_HASH_LENGTH HASH_LENGTH + 6
#define ENCODED_HASH_LENGTH 113
#define CID_LENGTH ENCODED_HASH_LENGTH + 2

void add_tnfs_file(char* filename)
{
    struct stat *buffer = malloc(sizeof(struct stat));
    int status = get_file_stats(filename, buffer);

    if(status != 0)
    {
        log_formated_error("The given file (%s) is not found.", filename);
        return;
    }

    if(buffer->st_size > MAX_FILE_LENGTH)
    {
        log_formated_error("The given file is too big (%ld bytes).", MAX_FILE_LENGTH);
        return;
    }

    if(buffer->st_size == 0)
    {
        log_error("The given file is empty.");
        return;
    }

    char* output = (char *)malloc(buffer->st_size);

    size_t output_length = get_file_content(filename, output);

    if(output_length == 0)
    {
        log_error("An error occured while reading the file.");
    }

    char* cid = (char*)malloc(sizeof(char) * CID_LENGTH);
    generate_cid_from_file_content(output, output_length, cid);

    log_formated_info("New file %s added to tnfs with CID %s!", filename, cid);

    free(output);
    free(cid);
    free(buffer);
}

void generate_cid_from_file_content(char* input, size_t input_length, char* cid)
{
    uint8_t *hash = (uint8_t *)malloc(sizeof(uint8_t) * SIZE_OF_SHA_256_HASH);
    calc_sha_256(hash, input, input_length);
    char *hash_string = (char *)malloc(sizeof(char) * HASH_LENGTH);
    hash_to_string(hash, hash_string);
    char* hash_headers = "011220";

    char* plain_hash = (char *)malloc(sizeof(char) * PLAIN_HASH_LENGTH);
    sprintf(plain_hash, "%s%s", hash_headers, hash_string);
    log_formated_debug("HashString: %s", hash_string);
    log_formated_debug("PlainHash: %s", plain_hash);
    char* encoded = (char *)malloc(sizeof(char) * ENCODED_HASH_LENGTH);

    base32_encode((const unsigned char*)plain_hash, PLAIN_HASH_LENGTH, (unsigned char*)encoded);
    log_formated_debug("Encoded: %s", encoded);

    sprintf(cid, "b%s", encoded);
    log_formated_debug("Cid: %s", cid);

    free(encoded);
    free(hash);
    free(hash_string);
    free(plain_hash);
}