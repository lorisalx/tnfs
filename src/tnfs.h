#include <stddef.h>
#ifndef TNFS_H
#define TNFS_H

#define MAX_FILE_LENGTH 102400000 // Maximum file size : 102.4mb
#define BLOCK_LIMIT 256000 // Maximum size of 1 block for the chunker : 256kb
#define MAX_LINKS 400

void add_tnfs_file(char* filename); // To add a file into tnfs system
#endif
