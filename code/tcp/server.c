#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <time.h>

#define SERVER_IP     "127.0.0.1"
#define SERVER_PORT   5001
#define BACKLOG       5
#define BUFFER_SIZE   1024


int configure_server() {
  int lsd;
  struct sockaddr_in server;

  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(SERVER_IP);
  server.sin_port = htons(SERVER_PORT);

  lsd = socket(AF_INET, SOCK_STREAM, 0);
  if (lsd < 0) {
    perror("socket");
    return -1;
  }

  if (bind(lsd, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("bind");
    return -1;
  }

  if (listen(lsd, BACKLOG)) {
    perror("lsiten");
    return -1;
  }

  printf("Server is listening at port %d\n", SERVER_PORT);
  return lsd;

}

void handle_client(const int nsd, const struct sockaddr_in client) {
  char buffer[BUFFER_SIZE] = {0};
  char* response;
 
  printf("\nClient %s:%d is connected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

  while (recv(nsd, buffer, BUFFER_SIZE, 0) > 0) {
    if (!strcmp(buffer, "exit")) {
      break;
    }

    printf("Client %s:%u => %s\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), buffer);

    if (!strcmp(buffer, "time")) {
      response = ctime(&(time_t){time(NULL)});
      response[strlen(response) - 1] = '\0';
    } else {
      response = "Bad Request";
    }

    if (send(nsd, response, strlen(response), 0) < 0) {
      perror("send"); 
    }

    memset(&buffer, 0, BUFFER_SIZE);
  }

  printf("Client %s:%d is disconnected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
}

int main () {
  int lsd, nsd;
  struct sockaddr_in client;

  lsd = configure_server();
  if (lsd == -1) {
    return 1;
  }

  while (1) {
    nsd = accept(lsd, (struct sockaddr*)&client, &(socklen_t){sizeof(client)});
    if (nsd == -1) {
      perror("accept");
    } else {
      handle_client(nsd, client);
    }
    close(nsd);
  }

  close(lsd);

  return 0;
}
