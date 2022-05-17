#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "peer.h"
#include "file.h"
#include "tnfs.h"
#include "logger.h"
#include "dag.h"
#include "redis.h"
#include "lib/crypt/sha-256.h"

void add_peer(Peer *p) {
    char *fip, *port;
    port = (char *)malloc(sizeof(char) * 5);
    fip = (char *)malloc(sizeof(char) * 30);
    sprintf(port, "%d", p->port); //itoa
    strcpy(fip, p->ip);
    strcat(fip, ":");
    strcat(fip,port);
    log_formated_info("The following peer has been added : %s",fip);
    set_redis_command(PEER,"customid",fip);
    free(port);
    free(fip);
}

Peer* get_peer(char *id) {
    char fip[30];
    get_redis_command(PEER, id, fip);

    if(strcmp(fip,"null") == 0) {
        log_formated_error("The following peer doesn't exist : %s.",id);
        exit(EXIT_FAILURE);
    }

    char *ip, *port;
    char *search = ":";

    ip = strtok(fip, search);
    port = strtok(NULL, search);

    log_formated_info("IP : %s",ip);
    log_formated_info("PORT : %s",port);

    Peer *p = calloc(1,sizeof(Peer));
    p->id = id;
    p->ip = ip;
    p->port = atoi(port);
    return p;
}

void get_all_peers() {
    char res[1000];
    keys_redis_command(PEER,res);
}