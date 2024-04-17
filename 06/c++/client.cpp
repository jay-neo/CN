#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 512


int main() {
    sleep(2);
    std::string message;
    std::string buffer(BUFFER_SIZE, '\0');

    struct sockaddr_in server;
    bzero((char*)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("connect");
        return 1;
    }

    do {
        sleep(1);
        std::cout << "\nClient >> Enter your message: ";
        std::cin >> message;
        
        ssize_t sent_bytes = send(sockfd, message.c_str(), message.length(), 0);
        if (sent_bytes < 0) {
            perror("send");
            return 1;
        }

        // memset(&buffer[0], 0, buffer.size());
        ssize_t received_bytes = recv(sockfd, &buffer[0], buffer.size(), 0);
        if (received_bytes < 0) {
            perror("recv");
            return 1;
        }

        std::cout << "Client >> " << buffer << std::endl;

    } while (message != "exit");

    close(sockfd);
    return 0;
}
