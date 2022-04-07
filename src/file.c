#include "file.h"
#include "logger.h"
#include "sha-256.h"
#include "base-32.h"

#include <stdio.h>
#include <stdlib.h>

size_t get_file_content(char* filename, char* output)
{
    FILE *f;

    if ((f = fopen(filename, "rb")) == NULL){
        log_error("An error occured while loading a file.");
        return 0;
    }
    
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    fread(output, fsize, 1, f);
    fclose(f);

    return fsize;
}

int get_file_stats(char* filename, struct stat *buffer)
{
    return stat(filename, buffer);
}

void generate_cid_from_file(char* filename, char* cid) 
{
    // Récupération du fichier
    struct stat *buffer = malloc(sizeof(struct stat));
    get_file_stats(filename, buffer);

    char* output = (char *) malloc(buffer->st_size * sizeof(char));
    size_t output_length = get_file_content(filename, output);

    // Génération du CID
    generate_cid_from_file_content(output, output_length, cid);

    free(buffer);
    free(output);
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