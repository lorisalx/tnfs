#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "network.h"
#include "logger.h"
#include "peer.h"

void send_file(FILE *fp, int sockfd)
{
    int n;
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

void tcp_send_file(Peer *p, char *filename)
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
}