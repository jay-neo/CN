#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // Include for close()
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

    int listening_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_sd < 0) {
        perror("socket");
        return 1;
    }

    if (bind(listening_sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(listening_sd, 5) < 0) {
        perror("listen");
        return 1;
    }

    std::cout << "Server is listening on port " << SERVER_PORT << std::endl;

    while (true) {
        int connection_sd = accept(listening_sd, (struct sockaddr *)&client, &clen);
        if (connection_sd < 0) {
            perror("accept");
            break;
        }

        std::cout << "\nServer >> " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " is connected" << std::endl;
        char* buffer = new char[BUFFER_SIZE];

        do {
            memset(buffer, 0x0, BUFFER_SIZE);

            int bytes_received = recv(connection_sd, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received < 0) {
                perror("recv");
                break;
            } else if (bytes_received == 0) {
                std::cout << "\nServer >> " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " is disconnected" << std::endl;
                break;
            }
            time_t t = time(NULL);
            
            {
                const char* response = !strcmp(buffer, "time") ? ctime(&t) : "Bad Request";
                ssize_t sent_bytes = send(connection_sd, response, strlen(response) + 1, 0);
                // std::string response = !strcmp(buffer, "time") ? ctime(&t) : "Bad Request";
                // ssize_t sent_bytes = send(connection_sd, response.c_str(), response.length() + 1, 0);
                if (sent_bytes < 0) {
                    perror("send");
                    break;
                }
            }

        } while (strcmp(buffer, "exit"));

        std::cout << "\nServer >> " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " is manually disconnected" << std::endl;

        delete[] buffer;
        close(connection_sd);
    }

    close(listening_sd);

    return 0;
}
