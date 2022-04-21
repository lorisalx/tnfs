#include "logger.h"
#include "tnfs.h"
#include "redis.h"
#include "dag.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void print_usage() {
    log_error(" Usage: tnfs <command> <param>");
}

int main(int argc, char *argv[]) {

    // Init redis
    init_redis();

    /*int opt= 0;

    static struct option long_options[] = {
        { "add", required_argument, 0, 'a' }
    };

    int long_index = 0;

    while ((opt = getopt_long(argc, argv, "a", long_options, NULL )) != -1) {
        printf("i = %i\n", opt);
        switch (opt) {
             case 'a' :
                add_tnfs_file((char *)optarg);
                break;
             default:
                print_usage();
                exit(EXIT_FAILURE);
        }
    }*/

    //add_tnfs_file("./V3.psd");
    tnfs_get_infos("bGAYTCMRSGA4WGMRTMZRGGNBQMRSWKMBZGA4GEMDBHE4TIMTDMFSGMMBWMUYWKMRXGYZTGMDDMNRWMODDMU3TIZJQGA4DGOBTHAZDCMRVMFRGGMBQ");

    dispose();
    return 0;
}
