#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "network.h"
#include "logger.h"
#include "peer.h"
#include "redis.h"
/*
void send_file(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd, data, sizeof(data), 0) == -1)
        {
            log_error("Error while sending the file");
            exit(EXIT_FAILURE);
        }
        bzero(data, SIZE);
    }
}

void write_file(int sockfd, char *filename)
{
    int n;
    FILE *fp;
    char buffer[SIZE];
    log_info("Fichier en cours d'écriture");
    fp = fopen(filename, "w+");
    while (1)
    {
        n = recv(sockfd, buffer, SIZE, 0);
        log_info("Fichier en cours d'écriture ...");

        if (n <= 0)
        {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
}
*/
/*void tcp_send_file(Peer *p, char *filename)
{

    char *ip = p->ip;
    int port = p->port;
    int e;
    log_formated_info("%s", ip);
    log_formated_info("%d", port);

    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        log_error("Error creating socket");
        exit(EXIT_FAILURE);
    }
    log_info("Server socket created successfully");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((e = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0)
    {
        log_formated_error("Error connecting socket %d", e);
        exit(EXIT_FAILURE);
    }
    log_info("Connected to the server");

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        log_error("Error while reading file");
        exit(EXIT_FAILURE);
    }

    send_file(fp, sockfd);
    log_info("File data sent successfully");

    log_info("Closing the connection");
    close(sockfd);
}*/

void look_for_block(char* cid) {
    // Récupération de tout les peers
    int results = 0;
    Peer** array = get_all_peers(&results);
    printf("On est là\n");
    for(int i = 0; i < results; i++) {
        // Filling server information 
        struct sockaddr_in servaddr; 
        memset(&servaddr, 0, sizeof(servaddr)); 
        servaddr.sin_family = AF_INET; 
        servaddr.sin_port = array[i]->port; 
        servaddr.sin_addr.s_addr = inet_addr(array[i]->ip);

        // Création d'un socket UDP
        int socket_UDP = create_socket(UDP_CONNECTION, NULL);
        printf("On envoi à : %s %d\n", array[i]->ip, array[i]->port);
        char *hello = "Hello from client"; 
        sendto(socket_UDP, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &servaddr,  sizeof(servaddr)); 
        /*sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); */

        close(socket_UDP);
        // Ouverture connexion UDP 
        // Envoie une demande
        // Si oui :
            // Ouverture en TCP
            // Telechargement du fichier
        // Si non :
            // Envoie une demande de peer
    }

    // Pour chaque pair dans la table de routage
   /* char buffer[1];
    Peer* ptab[MAX_REDIS_KEYS];
    *ptab = calloc(MAX_REDIS_KEYS,sizeof(Peer));
    get_all_peers(ptab);
    int i = 0;
    while(ptab[i] != NULL && i!=1) {
        // Voir si on a le fichier
        int sockfd = open_tcp_socket(ptab[i]);
        send_txt_tcp(sockfd,"00");
        send_txt_tcp(sockfd,cid);
		recv(sockfd, buffer, sizeof(buffer), 0);
        log_formated_info("Result : %s",buffer);
        int res = 1;
        if(strcmp(buffer,"1") == 0) {
            log_formated_info("Block %s has been found", cid);
            write_file(sockfd,"test");
            break;
        }
        else if(strcmp(buffer,"0") == 0) {
            log_info("Block not found on this peer");
        }
        close_tcp_socket(sockfd);
        i++;
    }*/
}

/*
void send_txt_tcp(int sockfd, char* txt) {
    log_formated_info("Text to be send : %s",txt);
    send(sockfd,txt,strlen(txt),0);
}*/


int create_socket(CONNECTION_TYPE type, struct sockaddr_in* servaddr) {
    int sockfd;

    if(type == UDP_CONNECTION) {
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
            log_error("Error creating socket");
            exit(EXIT_FAILURE); 
        } 
    } else if(type == TCP_CONNECTION) {
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
            log_error("Error creating socket");
            exit(EXIT_FAILURE); 
        } 
    }

    if(type == TCP_CONNECTION) {
        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
            log_error("Error TCP connection");
            exit(EXIT_FAILURE); 
        }
    }
    return sockfd;
}

void close_socket(int sockfd) {
    log_info("Closing the connection");
    close(sockfd);
}