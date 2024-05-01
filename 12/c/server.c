#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5001
#define BUFFER_SIZE 512
#define CHILDREN_NUM 3


// unnecessary things
#define MAX_QUERY 7
#define MAX_LENGTH 100
char query[MAX_QUERY][MAX_LENGTH] = {
    "What is your name?",
    "What is your college name?",
    "What is your favorite movie name?",
    "What is your favorite book name?",
    "What is your phone number?",
    "Enter 'exit' to say goodbye from the chat..."
};
char msg[MAX_QUERY / 2][MAX_LENGTH] = {
    "Hello ",
    "Best movie I ever seen is ",
    "10 minutes later I call you at "
};


int main() {
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int listening_sd, connection_sd;
    struct sockaddr_in client, server;
    int idx = 0;
    pid_t pid;

    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    listening_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_sd < 0) {
        perror("socket");
        return 1;
    }

    if (bind(listening_sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(listening_sd, 5) < 0) {
        perror("listen");
        return 1;
    }

    printf(">> Server is listening on port %u\n", SERVER_PORT);

    
    while (1) {
        connection_sd = accept(listening_sd, (struct sockaddr*) &client, &(socklen_t){sizeof(client)});
        if (connection_sd < 0) {
            perror("accept");
            continue;
        }
        
        idx = 0;
        pid = fork();
        if (pid < 0) {
            perror("Failed to fork");
            return 1;
        } else if (pid == 0) {
            
            printf(">> %s:%u is connected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            
            while (1) {
                memset(buffer, 0x0, BUFFER_SIZE);
                if (recv(connection_sd, buffer, BUFFER_SIZE - 1, 0) < 0) {
                    perror("recv");
                    continue;
                }
                
                
                if (!strcmp(buffer, "exit")) {
                    break;
                }

                strcpy(response, query[idx]);

                // unnecessary things
                if (idx & 1) {
                    strcpy(response, "\n🤖 ");
                    strcat(response, msg[(idx / 2)]);
                    strcat(response, buffer);
                    strcat(response, "\n\n");
                    strcat(response, query[idx]);
                }

                if (send(connection_sd, response, strlen(response) + 1, 0) < 0) {
                    perror("send");
                    break;
                }

                ++idx;
                if (idx == MAX_QUERY - 1) {
                    idx = 0;
                }
            }

            printf(">> %s:%u is disconnected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        }

        close(connection_sd);
    }

    close(listening_sd);

    return 0;
}
