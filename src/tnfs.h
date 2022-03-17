#include <stddef.h>
#ifndef TNFS_H
#define TNFS_H
/**
 * @brief 100 mb
 */
#define MAX_FILE_LENGTH 100000000

void add_tnfs_file(char* filename);
void generate_cid_from_file_content(char* input, size_t input_length, char* cid);
#endif //TNFS_H
