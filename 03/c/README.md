## PIPE

```c
#include <unistd.h>
int pipe(int pipefd[2]);
```

In Unix-like operating systems, a pipe is a mechanism that allows two processes to communicate with each other. It provides a one-way flow of data, where one process can write data to the pipe, and the other process can read that data from the pipe. Pipes are commonly used for inter-process communication (IPC), allowing processes to pass information between each other.

When a pipe is created, it is typically used in conjunction with the `fork` system call to create a parent process and a child process. Here's how pipes are used in the context of parent and child processes:

1. **Creating a Pipe**: Before forking, a pipe is created using the `pipe` system call. This creates a pair of file descriptors: one for reading from the pipe (`pipe_fd[0]`) and one for writing to the pipe (`pipe_fd[1]`).

2. **Forking**: After creating the pipe, the parent process calls `fork` to create a child process. Both the parent and child processes inherit the file descriptors associated with the pipe.

3. **Parent Process**: In the parent process, the parent typically closes the read end of the pipe (`pipe_fd[0]`) and writes data to the pipe using the write end of the pipe (`pipe_fd[1]`).

4. **Child Process**: In the child process, the child typically closes the write end of the pipe (`pipe_fd[1]`) and reads data from the pipe using the read end of the pipe (`pipe_fd[0]`).

5. **Data Transfer**: Once the pipe is set up and the processes are running, the parent process can write data to the pipe, and the child process can read that data from the pipe. This allows the processes to communicate and exchange information.

6. **Closing the Pipe**: After the communication is complete, both the parent and child processes should close their respective ends of the pipe (`pipe_fd[0]` and `pipe_fd[1]`) to release system resources.

Overall, pipes provide a simple and efficient way for parent and child processes to communicate in Unix-like operating systems. They are often used in conjunction with forking to implement various forms of IPC.

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