#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 512

int main() {
    int sd, nsd, flag = 1;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client, server;
    int clen = sizeof(client);

    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket");
        return 1;
    }

    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(sd, 5) < 0) {
        perror("listen");
        return 1;
    }

    printf("Server is listening on %s:%d\n", SERVER_IP, SERVER_PORT);

    while (flag) {
        nsd = accept(sd, (struct sockaddr *)&client, (socklen_t *)&clen);
        if (nsd < 0) {
            perror("accept");
            continue;
        }

        printf("\nServer >> %s:%u is connected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        do {
            memset(buffer, 0x0, BUFFER_SIZE);
            int bytes_received = recv(nsd, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received < 0) {
                perror("recv");
                break;
            } else if (bytes_received == 0) {
                printf("\nServer >> %s:%u is disconnected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                flag = 0;
                break;
            }
            buffer[bytes_received] = '\0'; // Ensure null-termination for printing
            send(nsd, buffer, strlen(buffer)+1, 0);
        } while (strcmp(buffer, "stop"));

        printf("\nServer >> %s:%u is manually disconnected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        close(nsd);
    }

    close(sd);

    return 0;
}
