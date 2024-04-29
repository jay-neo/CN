#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define SERVER_IP     "127.0.0.1"
#define SERVER_PORT   5000
#define MSG_SIZE      1024
#define BUFFER_SIZE   1024


void client_interface(const int sd, const struct sockaddr_in server) {
  char msg[MSG_SIZE], buffer[BUFFER_SIZE];

  while (1) {
    printf("Enter your message: ");
    scanf("%s", msg);

    if (sendto(sd, msg, strlen(msg), 0, (struct sockaddr*) &server, sizeof(server)) < 0) {
      perror("sendto");
      continue;
    }

    if (!strcmp(msg, "exit")) {
      break;
    }

    memset(buffer, 0, BUFFER_SIZE);
    if (recvfrom(sd, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &server, &(socklen_t){sizeof(server)}) < 0) {
      perror("recv");
    }

    printf("Server Response: %s\n", buffer);
  }

  printf("\nExiting from client side ...\n\n");
}


int main() {
  int sd;
  struct sockaddr_in server;

  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(SERVER_IP);
  server.sin_port = htons(SERVER_PORT);

  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    perror("socket");
    return 1;
  }

  client_interface(sd, server);

  close(sd);
  return 0;
}
