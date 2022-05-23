#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "network.h"
#include "logger.h"
#include "peer.h"
#include "redis.h"
#include "tnfs.h"

void write_file(int sockfd, char *cid)
{
    char file_path[PATH_MAX], file_name[NAME_MAX];
    //int fp;
    snprintf(file_name, sizeof(file_name), "%s.dat", cid);
    snprintf(file_path, sizeof(file_path), "%s%s", BLOCK_PATH, file_name);

    int receivedFile = open(file_path, O_RDWR | O_CREAT, 0777);

    // Récupération de la taille du fichier
    char buffer[SIZE];
    bzero(buffer, SIZE);

    if(recv(sockfd, buffer, SIZE, 0) < 0){
        close_socket(sockfd);
        exit(EXIT_FAILURE); 
    }

    int remain_data = atoi(buffer);

    // Récupération du fichier
    ssize_t len;

    while ((remain_data > 0) && ((len = recv(sockfd, buffer, SIZE, 0)) > 0)) {
        write(receivedFile, buffer, len);
        remain_data -= len;
    }

    // Adding the tuple (cid=>block_name) into redis
    set_redis_command(BLOCK, cid, file_name);

    close(receivedFile);
}

void look_for_block(char* cid) {

    // Récupération de tout les peers
    int results = 0;
    Peer** array = get_all_peers(&results);

    for(int i = 0; i < results; i++) {

        char buffer[SIZE];
        int socket_TCP = create_socket(array[i]);

        if(socket_TCP == -1) 
            continue;

        // Send the request :
        bzero(buffer, SIZE);
        snprintf(buffer, SIZE, "%s", cid);
        if(send_tcp(socket_TCP, buffer) == -1)
            continue;

        // Listen response :
        bzero(buffer, SIZE);
        if(recv(socket_TCP, buffer, SIZE, 0) < 0){
            close_socket(socket_TCP);
            continue;
        }

        // Response analyse :
        int typeResult = 0;
        if(strcmp(buffer, "Y") == 0){
            typeResult = 1 ;
            bzero(buffer, SIZE);
            snprintf(buffer, SIZE, "BLOCK");
        } else {
            typeResult = 2 ;
            bzero(buffer, SIZE);
            snprintf(buffer, SIZE, "PEERS");
        }

        // Send PEERS or BLOCK
        if(send_tcp(socket_TCP, buffer) == -1)
            continue;

        // Listen response
        if(typeResult == 1) {
            write_file(socket_TCP, cid);
            close_socket(socket_TCP);
            break;
        } else {
            printf("Il envoie les peers\n");
        }

        close_socket(socket_TCP);
    }
}


int send_tcp(int sockfd, char* buffer) {
    if(send(sockfd, buffer, SIZE, 0) < 0){
        close_socket(sockfd);
        return -1;
    }

    return 1;
}

int create_socket(Peer* p) {
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
        log_error("Error creating socket");
        exit(EXIT_FAILURE); 
    } 

    // Filling server information 
    struct sockaddr_in servaddr; 
    memset(&servaddr, 0, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(p->port); 
    servaddr.sin_addr.s_addr = inet_addr(p->ip);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        sockfd = -1;
    }

    return sockfd;
}

void close_socket(int sockfd) {
    // log_info("Closing the connection");
    close(sockfd);
}