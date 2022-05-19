#include "logger.h"
#include "tnfs.h"
#include "redis.h"

#include <string.h>
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
    log_error("   - peer add <IP>:<PORT>");
    log_error("   - peer remove <IP>:<PORT>");
    exit(EXIT_FAILURE);
}

void neededArgument(int argc, int needed) {
    if(argc < needed)
        print_usage();
}

int main(int argc, char *argv[])
{
    // Init redis
    if(init_redis() == -1) 
        return EXIT_FAILURE;


    // Need minimum 2 arguments
    neededArgument(argc, 2);

    // Read arguments
    if(strcmp("add", argv[1]) == 0) {
        neededArgument(argc, 3);
        add_tnfs_file(argv[2]);
    } else if (strcmp("get", argv[1]) == 0) {
        neededArgument(argc, 3);
        get_tnfs_file(argv[2]);
        log_info("File downloaded !");
    } else if (strcmp("infos", argv[1]) == 0) {
        neededArgument(argc, 3);
        tnfs_get_infos(argv[2]);
    } else if (strcmp("clean", argv[1]) == 0) {
        tnfs_clean_data();
    } else if (strcmp("peer", argv[1]) == 0) {
        neededArgument(argc, 4);

        if (strcmp("add", argv[2]) == 0) {
            tnfs_addpeer(argv[3]);
        } else if (strcmp("remove", argv[2]) == 0) {
            tnfs_removepeer(argv[3]);
        } else {
            print_usage();
        }
    } else {
        print_usage();
    }

    // Deconnect redis
    dispose();

    return EXIT_SUCCESS;
}