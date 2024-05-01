#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // Include for close()
#include <ctime>

namespace config {
    const char* SERVER_IP = "127.0.0.1";
    const uint16_t  SERVER_PORT = 5001;
    const int BUFFER_SIZE = 512;
}



void handle_client(const int &connection_sd, const struct sockaddr_in &client) {

    int BUFFER_SIZE = config::BUFFER_SIZE;
    char* buffer = new char[BUFFER_SIZE];

    std::cout << "\nServer >> " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " is connected" << std::endl;

    while (1) {
        memset(buffer, 0x0, BUFFER_SIZE);

        ssize_t bytes_received = recv(connection_sd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("recv");
            break;
        } else if (bytes_received == 0) {
            std::cout << "\nServer >> " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " is disconnected" << std::endl;
            break;
        }


        if (!strcmp(buffer, "exit")) {
            break;
        }

        time_t t = time(NULL);
        const char* response = !strcmp(buffer, "time") ? ctime(&t) : "Bad Request";
        ssize_t sent_bytes = send(connection_sd, response, strlen(response) + 1, 0);
        // std::string response = !strcmp(buffer, "time") ? ctime(&t) : "Bad Request";
        // ssize_t sent_bytes = send(connection_sd, response.c_str(), response.length() + 1, 0);
        if (sent_bytes < 0) {
            perror("send");
            break;
        }

    }
    delete[] buffer;

    std::cout << "\nServer >> " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " is manually disconnected" << std::endl;
}


int main() {
    struct sockaddr_in client, server;
    socklen_t clen = sizeof(client);

    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(config::SERVER_IP);
    server.sin_port = htons(config::SERVER_PORT);

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

    std::cout << "Server is listening on port " << config::SERVER_PORT << std::endl;

    while (true) {
        int connection_sd = accept(listening_sd, (struct sockaddr *)&client, &clen);
        if (connection_sd < 0) {
            perror("accept");
        } else {
            handle_client(connection_sd, client);
        }
        close(connection_sd);
    }

    close(listening_sd);

    return 0;
}
