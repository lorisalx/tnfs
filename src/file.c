#include "file.h"
#include <stdio.h>
#include "logger.h"

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

    output = (char*)malloc(fsize + 1);
    fread(output, fsize, 1, f);
    fclose(f);

    output[fsize] = 0;
    return fsize;
}

int get_file_stats(char* filename, struct stat *buffer)
{
    return stat(filename, buffer);
}