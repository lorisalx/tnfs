#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <sys/stat.h>
/**
 * @brief Read all the content from a given file
 * @param filename The filename to read
 * @param output The output data pointer
 * @return size_t The length of the ouptput
 */
size_t get_file_content(char* filename, char* output);
int get_file_stats(char* filename, struct stat *buffer);
#endif //FILE_H
