#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 512

void* handle_client(void *arg) {
    int connection_sd = *(int *)arg;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received, bytes_sent;

    do {
        memset(buffer, 0x0, BUFFER_SIZE);
        bytes_received = recv(connection_sd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("recv");
            break;
        } else if (bytes_received == 0) {
            break;
        }
        buffer[bytes_received] = '\0';
        
        bytes_sent = send(connection_sd, buffer, strlen(buffer) + 1, 0);
        if (bytes_sent < 0) {
            perror("send");
            continue;
        }
    } while (strcmp(buffer, "exit"));

    close(connection_sd);
    printf("\nServer >> %s:%u disconnected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    pthread_exit(NULL);
}

int main() {
    int listening_sd, connection_sd;
    struct sockaddr_in client, server;
    pthread_t tid;
    
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

    printf("Server is listening on port %u\n", SERVER_PORT);

    while (1) {
        connection_sd = accept(listening_sd, (struct sockaddr*) &client, &(socklen_t){sizeof(client)});
        if (connection_sd < 0) {
            perror("accept");
            continue;
        }
        printf("\nServer >> %s:%u connected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        // Multithreading Implementation
        if (pthread_create(&tid, NULL, handle_client, &connection_sd) != 0) {
            perror("pthread_create");
            close(connection_sd);
        }
    }

    close(listening_sd);

    return 0;
}
