#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 1024


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
        close(sockfd);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    while (1) {
        sleep(1);
        std::cout << "\nClient >> Enter your message: ";
        std::cin >> message;
        
        if (send(sockfd, message.c_str(), message.length(), 0) < 0) {
            perror("send");
            continue;
        }

        if (message == "exit") {
            std::cout << "Client >> Manual terminating from client side" << std::endl;
            break;
        }
        
        memset(&buffer[0], 0, buffer.size());
        if (recv(sockfd, &buffer[0], buffer.size(), 0) < 0) {
            perror("recv");
            continue;
        }
        
        std::cout << "Client >> " << buffer << std::endl;

    }
    
    close(sockfd);
    return 0;
}
