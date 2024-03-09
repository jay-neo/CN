#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
/*
pipe_fd[0]  ---> read
pipe_fd[1]  ---> write
*/

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];
    int n, fd, pipe_fd[2], pid;

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];

    fd = open(filename, O_RDONLY, 0764);
    if (fd == -1) {
        perror("Failed to open file");
        exit(1);
    }

    n = read(fd, buffer, sizeof(buffer));
    if (n == -1) {
        perror("Failed to read file");
        exit(1);
    }

    if (pipe(pipe_fd) == -1) {
        perror("Failed to create pipe");
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("Failed to fork process");
        exit(1);

    } else if (pid > 0) {
        printf("\nParent Process\n");
        close(pipe_fd[0]);
        if (write(pipe_fd[1], buffer, n) == -1) {
            perror("Failed to write to pipe");
            exit(1);
        }

    } else if (pid == 0) {
        printf("\nChild Process\n");
        close(pipe_fd[1]);
        n = read(pipe_fd[0], buffer, sizeof(buffer));
        if (n == -1) {
            perror("Failed to read from pipe");
            exit(1);
        }
        printf("\nReading from pipe:\n");
        puts(buffer);
    }

    close(fd);
    return 0;
}
