#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 512

int main() {
    int sd, nsd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client, server;
    int clen = sizeof(client);

    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sd, (struct sockaddr*)&server, sizeof(server));
    listen(sd, 5);
    printf("Server is listening on port %d\n", SERVER_IP, SERVER_PORT);
    
    while (1) {
        nsd = accept(sd, (struct sockaddr*) &client, &clen);
        do {
            memset(buffer, 0x0, BUFFER_SIZE);
            recv(nsd, buffer, BUFFER_SIZE, 0);
            printf("\nClient's IP = %s\n", inet_ntoa(client.sin_addr));
            printf("Client's Port = %u\n", ntohs(client.sin_port));
            printf("Client's message = %s\n", buffer);
        } while (strcmp(buffer, "stop"));

        close(nsd);
    }

    close(sd);
    return 0;
}
