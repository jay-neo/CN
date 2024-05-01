#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>


namespace config {
    const char* SERVER_IP = "127.0.0.1";
    const uint16_t SERVER_PORT = 5000;
    const int BACKLOG = 5;
    const size_t BUFFER_SIZE = 1024;
}


int configure_tcp() {
    struct sockaddr_in server;

    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(config::SERVER_IP);
    server.sin_port = htons(config::SERVER_PORT);

    int listening_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_sd < 0) {
        perror("socket");
        return -1;
    }

    if (bind(listening_sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        return -1;
    }

    if (listen(listening_sd, config::BACKLOG) < 0) {
        perror("listen");
        return -1;
    }

    std::cout << "Server is listening on port " << config::SERVER_PORT << std::endl;
    return listening_sd;
}



void handle_tcp_client(const int connection_sd, const struct sockaddr_in &client) {

    int BUFFER_SIZE = config::BUFFER_SIZE;
    char* buffer = new char[BUFFER_SIZE]();

    std::cout << "\nServer >> " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " is connected" << std::endl;

    while (recv(connection_sd, buffer, BUFFER_SIZE - 1, 0) > 0) {

        if (!strcmp(buffer, "exit")) {
            break;
        }

        if (send(connection_sd, buffer, strlen(buffer) + 1, 0) < 0) {
            perror("send");
            break;
        }
        
        memset(buffer, 0x0, BUFFER_SIZE);

    }

    delete[] buffer;
    close(connection_sd);

    std::cout << "\nServer >> " << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " is disconnected" << std::endl;
    
}



int main() {

    int listening_sd = configure_tcp();
    if (listening_sd < 0) {
        close(listening_sd);
        return 1;
    }

    while (true) {
        struct sockaddr_in client;
        socklen_t clen = sizeof(client);

        int connection_sd = accept(listening_sd, (struct sockaddr *)&client, &clen);
        if (connection_sd < 0) {
            perror("accept");
            close(connection_sd);
            continue;
        }

        // Multithreaded Implementation
        std::thread(handle_tcp_client, connection_sd, client).detach();
    }
    
    close(listening_sd);

    return 0;
}