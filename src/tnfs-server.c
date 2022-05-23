#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <limits.h>
#include <sys/sendfile.h>

#include "logger.h"
#include "redis.h"
#include "file.h"
#include "dag.h"

#define SIZE 1024
#define MAX_CONNEXION 10
#define PORT 5050
#define BLOCK_PATH "./blocks/"

void send_file(char* filepath, int sockfd)
{
    FILE *f = open(filepath, 0);

    /* Get file stats */
    struct stat *stats = malloc(sizeof(struct stat));
    get_file_stats(filepath, stats);

    /* Send file size */
    char buffer[SIZE];
    bzero(buffer, SIZE);
    sprintf(buffer, "%ld", stats->st_size);

    if (send(sockfd, buffer, sizeof(buffer), 0) < 0){
        printf("Can't send\n");
        return -1;
    }

    /* Send file */
    int sent_bytes, remain_data;
    off_t offset = 0;

    remain_data = stats->st_size;

    while ((sent_bytes = sendfile(sockfd, f, &offset, BUFSIZ)) > 0 && (remain_data > 0))
    {
        remain_data  -= sent_bytes;
    }

    free(stats);
}

int main()
{
    // Init redis
    if(init_redis() == -1) 
        return EXIT_FAILURE;

    // Init socket
    int serverSocket = init_socket();

    // Client socket id
    int clientSocket;
 
    // Client Address
    struct sockaddr_in cliAddr;
    socklen_t addr_size;
 
    // Child process id
    pid_t childpid;

    while(1) {
		clientSocket = accept(serverSocket, (struct sockaddr*)&cliAddr, &addr_size);

		if(clientSocket < 0){
			exit(1);
		}
		
		if((childpid = fork()) == 0){
			close(serverSocket);

            char buffer[SIZE];
            bzero(buffer, SIZE);
            
            // Receive wanted CID
            if (recv(clientSocket, buffer, SIZE, 0) < 0){
                printf("Couldn't receive\n");
                return -1;
            }

            log_formated_info("Looking for CID : %s", buffer);

            // Vérification de l'existence du block
            char block_name[NAME_MAX];
            get_redis_command(BLOCK, buffer, &block_name);

            /* Get file */
            char file_path[PATH_MAX];

            if(strcmp(block_name,"null") != 0) {
                snprintf(file_path, sizeof(file_path), "%s%s", BLOCK_PATH, block_name);

                // Vérification de l'existence du block
                FILE *f;
                if ((f = open(file_path, 0)) == NULL){
                    del_redis_command(BLOCK, buffer);
                    bzero(buffer, SIZE);
                    snprintf(buffer, SIZE, "N");
                } else {
                    // Vérification du CID
                    int verification = verify_block(file_path, buffer);
                    bzero(buffer, SIZE);

                    // Vérification de la validité du block
                    if(verification == -1) {
                        del_redis_command(BLOCK, buffer);
                        snprintf(buffer, SIZE, "N");
                        remove(file_path);
                    } else {
                        snprintf(buffer, SIZE, "Y");
                    }
                }
            } else {
                bzero(buffer, SIZE);
                snprintf(buffer, SIZE, "N");
            }

            if (send(clientSocket, buffer, sizeof(buffer)  - 1, 0) < 0){
                printf("Can't send\n");
                return -1;
            }

            // Want peer / block
            bzero(buffer, SIZE);
            if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0){
                printf("Couldn't receive\n");
                return -1;
            }

            if(strcmp(buffer,"BLOCK") == 0) {
                send_file(file_path, clientSocket);
            } else if(strcmp(buffer,"PEERS") == 0) {
                // Envoi des peers
            }

            close(clientSocket);
		}
	}

    // Close socket
    close(clientSocket);
    close(serverSocket);

    // Deconnect redis
    dispose();

    return 0;
}

int init_socket() {
    int serverSocket;
    struct sockaddr_in serverAddr;

    // Error handling
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
        log_error("Creation of listening TCP socket failed.");
        exit(EXIT_FAILURE);
    }
 
    // Server address
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Error handling
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        log_error("Binding of listening TCP socket failed.");
        exit(EXIT_FAILURE);
    }
 
    // Listening
    if (listen(serverSocket, MAX_CONNEXION) == 0) {
        log_formated_info("Listening on port %d...",PORT);
    }

    return serverSocket;
}