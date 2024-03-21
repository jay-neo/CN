#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define MSG_SIZE 512

int main() {
    sleep(2);

    int sd;
    ssize_t bytes_sent;
    char msg[MSG_SIZE];
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
    
    do {
        sleep(1);
        printf("\nClient >> Enter your message: ");
        // fgets(msg, MSG_SIZE, stdin); // Error
        scanf("%s", msg);

        bytes_sent = send(sd, msg, strlen(msg)+1, 0);
        if (bytes_sent < 0) {
            perror("send");
            break;
        }
    } while (strcmp(msg, "exit"));

    close(sd);
    return 0;
}
