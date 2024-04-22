#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 512

int main() {
    struct sockaddr_in client, server;
    int clen = sizeof(client);
    int sd, nsd;
    char buffer[BUFFER_SIZE];
    char * response; 

    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sd, (struct sockaddr*)&server, sizeof(server));
    listen(sd, 5);

    printf("The server is running on port %d\n", SERVER_PORT);
    
    while (1) {
        nsd = accept(sd, (struct sockaddr*) &client, &clen);
        printf(">> %s:%u is connected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        do {
            memset(buffer, 0x0, BUFFER_SIZE);
            recv(nsd, buffer, BUFFER_SIZE, 0);
            if (!strcmp(buffer, "time")) {
                response = ctime(&(time_t){time(NULL)});
            } else {
                response = "Bad Request :(";
            }
            send(nsd, response, strlen(response)+1, 0);

        } while (strcmp(buffer, "stop"));
        printf(">> %s:%u is disconnected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        close(nsd);
    }

    close(sd);
    return 0;
}