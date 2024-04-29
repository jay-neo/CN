#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP     "127.0.0.1"
#define SERVER_PORT   5000
#define MSG_SIZE      1024
#define BUFFER_SIZE   1024


int connect_server() {
  int sd;
  struct sockaddr_in server;

  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(SERVER_IP);
  server.sin_port = htons(SERVER_PORT);

  sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    perror("socket");
    return -1;
  }

  if (connect(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connect");
    return -1;
  }

  return sd;
}


void client_interface(const int server) {
  char msg[MSG_SIZE], buffer[BUFFER_SIZE];

  while (1) {
    printf("Enter your message: ");
    scanf("%s", msg);

    if (send(server, msg, strlen(msg), 0) < 0) {
      perror("send");
      continue;
    }

    if (!strcmp(msg, "exit")) {
      break;
    }

    memset(buffer, 0, BUFFER_SIZE);
    if (recv(server, buffer, BUFFER_SIZE, 0) < 0) {
      perror("recv");
    }

    printf("Server Response: %s\n", buffer);
  }

  printf("\nExiting from client side ...\n\n");
}


int main() {
  int server;

  server = connect_server();
  if (server == -1) {
    return 1;
  }

  client_interface(server);

  close(server);
  return 0;
}
