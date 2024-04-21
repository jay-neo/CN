#include "header.h"
#define MSG_SIZE 512

int main() {
    sleep(2);

    int sd;
    ssize_t bytes_sent, bytes_received;
    char msg[MSG_SIZE], buffer[BUFFER_SIZE];
    struct sockaddr_in server;

    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("socket");
        return 1;
    }

    if (connect(sd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("connect");
        return 1;
    }
    
    do {
        sleep(1);
        printf("\nClient >> Enter your message: ");
        scanf("%s", msg);

        bytes_sent = sendto(sd, msg, strlen(msg) + 1, 0, (struct sockaddr*) &server, sizeof(server));
        if (bytes_sent < 0) {
            perror("send");
            break;
        }

        memset(buffer, 0x0, BUFFER_SIZE);
        bytes_received = recvfrom(sd, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &server, &(socklen_t){sizeof(server)});
        if (bytes_received < 0) {
            perror("recv");
            break;
        } else if (bytes_received == 0) {
            printf("\nClient >> Server disconnected\n");
            break;
        }
        buffer[bytes_received] = '\0';

        printf("Client >> The Echo response: %s\n", buffer);
    } while (strcmp(msg, "exit"));

    close(sd);
    return 0;
}