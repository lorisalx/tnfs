#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <sys/stat.h>

#define HASH_LENGTH SIZE_OF_SHA_256_HASH * 2
#define PLAIN_HASH_LENGTH HASH_LENGTH + 6

#define ENCODED_HASH_LENGTH 113
#define CID_LENGTH ENCODED_HASH_LENGTH + 1

size_t get_file_content(char* filename, char* output); // Get file content
int get_file_stats(char* filename, struct stat *buffer); // Get file stats
void generate_cid_from_file(char* filename, char* cid); // Get CID from a file
void generate_cid_from_file_content(char* input, size_t input_length, char* cid); // Get CID from a content
#endif //FILE_H
