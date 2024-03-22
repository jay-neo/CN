#include "header.h"

int main() {
    char buffer[BUFFER_SIZE];
    int listening_sd;
    struct sockaddr_in client, server;
    ssize_t bytes_received, bytes_sent;

    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    listening_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (listening_sd < 0) {
        perror("socket");
        close(listening_sd);
        return 1;
    }

    if (bind(listening_sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        close(listening_sd);
        return 1;
    }

    printf("Server is listening on port %u\n", SERVER_PORT);

    while (1) {
        memset(buffer, 0x0, BUFFER_SIZE);
        bytes_received = recvfrom(listening_sd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&client, &(socklen_t){sizeof(client)});
        if (bytes_received < 0) {
            perror("recvfrom");
            continue;
        } else if (bytes_received == 0) {
            printf("\nServer >> Error occurred to %s:%u\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            continue;
        }

        printf("\nServer >> %s:%u sent a message\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        buffer[bytes_received] = '\0';
        
        bytes_sent = sendto(listening_sd, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&client, sizeof(client));
        if (bytes_sent < 0) {
            perror("sendto");
            continue;
        }
            
    }

    close(listening_sd);

    return 0;
}
