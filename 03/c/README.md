## PIPE

```c
#include <unistd.h>
int pipe(int pipefd[2]);
```

In Unix-like operating systems, a pipe is a fundamental mechanism for inter-process communication (IPC). They allow data to flow from one process to another in a unidirectional or bidirectional manner. signals and pipes support IPC for local usage. There are several types of pipes, each with different characteristics and use cases. Here are the main types of pipes:


### Unamed Pipe (Anonymous Pipe):

- Unidirectional: Data flows in one direction only.
- In-Memory: Exist only in memory and are not associated with any file system objects.
- Parent-Child Communication: Typically used for communication between a parent process and its child process.
- The pipe system call creates a pipe and returns two file descriptors: `int pipe(int pipefd[2]);`
   - One File Descriptor for the **read end** of the pipe: `pipefd[0]`
   - Another File Descriptor for the **write end** of the pipe: `pipefd[1]`
- When a pipe is created, it is typically used in conjunction with the `fork` system call to create a parent process and a child process.
- Use Cases:
   - Simple IPC between a parent and child process, such as redirecting output or input between processes.
- Reference: [Link](https://www.youtube.com/watch?v=sgFx6br_kB8)

```c
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[128];

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork the process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Child process
        close(pipefd[1]); // Close unused write end

        read(pipefd[0], buffer, sizeof(buffer)); // Read from the pipe
        printf("Child received: %s\n", buffer);

        close(pipefd[0]); // Close read end
    } else {
        // Parent process
        close(pipefd[0]); // Close unused read end

        const char *msg = "Hello from parent!";
        write(pipefd[1], msg, strlen(msg) + 1); // Write to the pipe

        close(pipefd[1]); // Close write end
    }

    return 0;
}

```


### Named Pipe (FIFO):

- Bidirectional: Can be used for two-way communication.
- Named: Associated with a name in the file system, which can be accessed by unrelated processes.
- Persistence: The named pipe exists in the file system as a special file.
- Use Cases:
   - IPC between unrelated processes that do not share a common ancestor.
   - Long-lasting IPC mechanisms where pipes need to exist beyond the lifespan of the processes.
- Reference: [[Link-1]](https://www.youtube.com/watch?v=NHs42-rJeWU) [[Link-2]](https://www.youtube.com/watch?v=tPMmazzn5tM)

```sh
# Create a named pipe
mkfifo myfifo

# Write to the pipe in one terminal
echo "Hello from terminal 1" > myfifo

# Read from the pipe in another terminal
cat myfifo

```



---

## FORK

```c
#include <unistd.h>
pid_t fork(void);
```

Forking is a concept in operating systems where a process creates a copy of itself. The new process is called the child process, and the original process is called the parent process. The child process is an exact copy of the parent process, except for a few differences such as the process ID (`pid`). Forking is commonly used for parallel processing, multitasking, and creating new processes in Unix-like operating systems.

When a fork occurs, the operating system creates a new address space for the child process, which is a copy of the parent process's address space. This means that the child process has its own copy of the parent's variables, stack, and heap. However, changes made to these memory areas in one process do not affect the other process.

After forking, both the parent and child processes continue executing from the point of the fork. The `fork` system call returns different values in the parent and child processes:

- In the parent process, `fork` returns the process ID (`pid`) of the child process.
- In the child process, `fork` returns 0.

By checking the return value of `fork`, a program can determine whether it is executing in the parent or child process and can take different actions accordingly. For example, a common pattern is to perform different tasks in the parent and child processes after forking.


---

## Output Explanation

The given output is produced by a program that demonstrates inter-process communication using pipes in a Unix-like environment. Here's an explanation of each part of the output:

1. **Parent Process**:
   - The program starts by forking a child process from the parent process.
   - The `if (pid > 0)` branch is executed in the parent process because `pid` is greater than 0 in the parent process after forking.
   - The message "Parent Process" is printed to indicate that this part of the code is executed in the parent process.
   - The parent process closes the read end of the pipe (`pipe_fd[0]`) because it only needs to write data to the pipe.

2. **Child Process**:
   - In the child process, the `if (pid == 0)` branch is executed because `pid` is 0 in the child process after forking.
   - The message "Child Process" is printed to indicate that this part of the code is executed in the child process.
   - The child process closes the write end of the pipe (`pipe_fd[1]`) because it only needs to read data from the pipe.

3. **Reading from pipe**:
   - After closing the write end of the pipe, the child process reads data from the pipe (`pipe_fd[0]`) using the `read` function.
   - The data read from the pipe is stored in the buffer `buffer`.
   - The child process then prints the message "Reading from pipe:" to indicate that it is reading data from the pipe.
   - Finally, the child process prints the contents of `buffer`, which contains the string "Hello from sample.txt file". This string was written to the pipe by the parent process earlier.

In summary, the program demonstrates how data can be passed from a parent process to a child process using a pipe. The parent process writes data to the pipe, and the child process reads the data from the pipe and prints it to the console.