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
#define MSG_SIZE 512

int main() {
    sleep(2);

    int sd;
    ssize_t bytes_sent, bytes_received;
    char msg[MSG_SIZE] = "Hello", buffer[BUFFER_SIZE];
    struct sockaddr_in server;

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
    
    printf("Wellcome to neo-server services 🙏\n");

    while (1) {
        sleep(1);
        if (send(sd, msg, strlen(msg) + 1, 0) < 0) {
            perror("send");
            break;
        }

        if (!strcmp(msg, "exit")) {
            break;
        }

        memset(buffer, 0x0, BUFFER_SIZE);
        if (recv(sd, buffer, BUFFER_SIZE, 0) <= 0) {
            perror("recv");
            break;
        }
        printf("%s\n>> ", buffer);

        fgets(msg, sizeof(msg), stdin);
        size_t length = strlen(msg);
        if (length > 0 && msg[length - 1] == '\n') {
            msg[length - 1] = '\0';
        }

    }

    close(sd);
    return 0;
}