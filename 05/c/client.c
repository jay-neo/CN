#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define CLIENT_IP "127.0.0.1"
#define CLIENT_PORT 7500
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define MSG_SIZE 512
#define BUFFER_SIZE 512

int main() {
    sleep(2);
    int sd;
    char msg[MSG_SIZE], buffer[BUFFER_SIZE];
    struct sockaddr_in client, server;

    bzero((char*)&client, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(CLIENT_IP);
    client.sin_port = htons(CLIENT_PORT);

    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    sd = socket(AF_INET, SOCK_STREAM, 0);
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
        send(sd, msg, strlen(msg)+1, 0);
        memset(buffer, 0x0, BUFFER_SIZE);
        recv(sd, buffer, BUFFER_SIZE, 0);
        printf("Client >> The Echo response: %s\n", buffer);
    } while (strcmp(msg, "stop"));

    close(sd);
    return 0;
}
