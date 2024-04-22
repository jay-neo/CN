#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define MSG_SIZE 512
#define BUFFER_SIZE 512

int main() {
    struct sockaddr_in client, server;
    char msg[MSG_SIZE], buffer[BUFFER_SIZE];
    int sd;

    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    connect(sd, (struct  sockaddr*)&server, sizeof(server));

    do{
        printf("Type 'time' to see currenet time: ");
        scanf("%s", msg);
        send(sd, msg, strlen(msg)+1, 0);

        memset(buffer, 0x0, BUFFER_SIZE);
        recv(sd, buffer, BUFFER_SIZE, 0);
        printf("%s\n", buffer);

    } while (strcmp(msg, "stop"));

    close(sd);
    return 0;
}