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
    // Formatage du Peer
    char *fip = peer_to_string(p);

    log_formated_info("The following peer has been added : %s", fip);
    set_redis_command(PEER,fip,"null");

    free(fip);
}

void remove_peer(Peer *p) {
    // Formatage du Peer
    char *fip = peer_to_string(p);

    log_formated_info("The following peer has been removed : %s", fip);
    del_redis_command(PEER, fip);

    free(fip);
}

Peer** get_all_peers(int* results) {
    // Récupération de tout les peers
    int nb = 0;
    char** array = get_all_peers_command(&nb);

    // Table de retour
    Peer **peerTab = calloc(nb, sizeof(Peer *));   
    *results = nb;

    // Création de l'ensemble des peers
    char *delimiter = ":";

    if(nb != 0) {
        for(int i = 0; i < nb; i++) {
            char *ip, *port;
            ip = strtok(array[i], delimiter);
            port = strtok(NULL, delimiter);

            Peer* p = malloc(sizeof(Peer));
            p->ip = ip;
            p->port = atoi(port);
            peerTab[i] = p;
        }
    }

    free(array);
    return peerTab;
}

char* peer_to_string(Peer* p) {
    char *port, *fip;
    port = (char *) malloc(PORT_SIZE);
    fip = (char *) malloc(IP_SIZE + PORT_SIZE + 1);

    // Formattage pour obtenir la forme <IP>:<PORT>
    sprintf(port, "%d", p->port);
    strcpy(fip, p->ip);
    strcat(fip, ":");
    strcat(fip, port);

    free(port);
    return fip;
}