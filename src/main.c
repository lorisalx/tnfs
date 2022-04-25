#include "logger.h"
#include "tnfs.h"
#include "redis.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void print_usage() {
    log_error(" Usage: tnfs <command> <param>");
    log_error(" Commands :");
    log_error("   - add <filename> : add a file to TNFS");
    log_error("   - get <CID> : get a file from TNFS");
    log_error("   - infos <CID> : get infos about a CID");
    log_error("   - clean : clean local data");
}

int main(int argc, char *argv[])
{
    // Init redis
    init_redis();

    if(argc < 2) {
        print_usage();
        return EXIT_FAILURE;
    }

    if(strcmp("add", argv[1]) == 0) {
        if(argc < 3)   {
            print_usage();
            return EXIT_FAILURE;
        }
        add_tnfs_file(argv[2]);
    } else if (strcmp("get", argv[1]) == 0) {
        if(argc < 3)   {
            print_usage();
            return EXIT_FAILURE;
        }

        get_tnfs_file(argv[2]);
        log_info("File downloaded !");
    } else if (strcmp("infos", argv[1]) == 0) {
        if(argc < 3)   {
            print_usage();
            return EXIT_FAILURE;
        }

        tnfs_get_infos(argv[2]);
    } else if (strcmp("clean", argv[1]) == 0) {
        tnfs_clean_data();
        log_info("Data cleaned !");
    } else {
        print_usage();
        return EXIT_FAILURE;
    }

    // Deconnect redis
    dispose();

    return EXIT_SUCCESS;
}