#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 512

int main() {
    struct sockaddr_in client, server;
    socklen_t clen = sizeof(client);

    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    int listening_sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (listening_sd < 0) {
        perror("socket");
        return 1;
    }

    if (bind(listening_sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        close(listening_sd);
        return 1;
    }

    std::cout << "Server is listening on port " << SERVER_PORT << std::endl;
    char* buffer = new char[BUFFER_SIZE];

    while (true) {

        memset(buffer, 0x0, BUFFER_SIZE);

        int bytes_received = recvfrom(listening_sd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&client, &clen);
        if (bytes_received < 0) {
            perror("recvfrom");
            break;
        } else if (bytes_received == 0) {
            std::cout << "\nServer >> " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " is disconnected" << std::endl;
            continue;
        }
        time_t t = time(NULL);
        
        const char* response = !strcmp(buffer, "time") ? ctime(&t) : "Bad Request";
        ssize_t sent_bytes = sendto(listening_sd, response, strlen(response) + 1, 0, (struct sockaddr*)&client, sizeof(client));
        if (sent_bytes < 0) {
            perror("sendto");
            continue;
        }
    }
    
    delete[] buffer;
    close(listening_sd);

    return 0;
}
