#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

#define BUFFER_SIZE 1024

int main() {
    char s1[BUFFER_SIZE], s2[BUFFER_SIZE];
    int n, fd1, fd2[2], pid;

    fd1 = open("sample.txt", O_RDONLY, 0764);
    n = read(fd1, s1, sizeof(s1));
    pipe(fd2);
    pid = fork();

    if (pid==-1) {
        printf("\nUnsucessful process creation\n");
        exit(1);
    } else if (pid>0) {
        printf("\nParent Process\n");
        close(fd2[0]);
        write(fd2[1], s1, n);
    } else if (pid==0) {
        printf("\nChild Process\n");
        close(fd2[1]);
        n = read(fd2[0], s2, sizeof(s2));
        printf("\nReading from pipe:\n");
        puts(s2);
    }
}

