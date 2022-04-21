#include <stddef.h>
#ifndef TNFS_H
#define TNFS_H

#define MAX_FILE_LENGTH 102400000 // Maximum file size : 102.4mb
#define BLOCK_LIMIT 256000 // Maximum size of 1 block for the chunker : 256kb
#define MAX_LINKS MAX_FILE_LENGTH/BLOCK_LIMIT
#define BLOCK_PATH "./blocks/"

void add_tnfs_file(char* filename); // To add a file into tnfs system
void tnfs_get_infos(char* cid); // Get infos about a CID
void get_tnfs_file(char* cid); // To download a file

#endif
