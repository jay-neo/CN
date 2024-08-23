# Socket Programming

## `<stdio.h>`

### `errno` and `perror()`

#### `errno`

The `<errno.h>` header file defines the integer variable `errno`, which is set by system calls and some library functions in the event of an error to indicate what went wrong.

The value in `errno` is significant only when the return value of the call indicated an error (i.e., -1 from most system calls; -1 or NULL from most library functions); a function that succeeds is allowed to change `errno`. The value of `errno` is never set to zero by any system call or library function.

`errno` is defined by the ISO C standard to be a modifiable `lvalue` of type `int`, and must not be explicitly declared; `errno` may be a macro. `errno` is thread-local; setting it in one thread does not affect its value in any other thread.

1. **Setting `errno`**: When a function encounters an error, it sets `errno` to a specific value that represents the type of error. For example, if a file operation fails due to a nonexistent file, `errno` might be set to `ENOENT` (which is typically an integer constant defined in a system header file).
2. **Using `errno`**: After a function call that may set `errno`, you can check its value to determine if an error occurred. If `errno` is nonzero, it indicates that an error occurred. The specific value of `errno` can then be used to identify the type of error.
3. **`strerror(errno)`**: The `strerror` function, declared in `<string.h>`, takes an error number (`errno` in this case) and returns a pointer to a string describing the error. This string is typically a human-readable description of the error. For example, if `errno` is set to `ENOENT`,

For more --> [Link]([errno(3) - Linux manual page (man7.org)](https://man7.org/linux/man-pages/man3/errno.3.html))

#### `perror()`

`perror` is a standard C library function used for printing error messages to the standard error stream (`stderr`). It is typically used to display a message corresponding to the current value of the `errno` variable, which is set by various system calls and library functions when an error occurs.

```c
void perror(const char *s);
```

- `s`: A pointer to a null-terminated string that is prepended to the error message. This string is usually used to indicate the context of the error message.
- `perror` takes the string pointed to by `s` and prints it, followed by a colon and a space (`": "`).
- It then prints the error message corresponding to the current value of `errno`, obtained by calling `strerror(errno)`.
- Finally, it prints a newline character (`'\n'`) to end the line.

## `<string.h>`

### Byte Manipulation Functions

#### System-V Unix System

```c
#include <strings.h>


// Set a block of memory to a specified value
void* memset(void* ptr, int value, size_t num);


// Copy a block of memory from one location to another
void* memcpy(void* destination, const void* source, size_t num);


// Copy a block of memory, handling overlapping regions correctly
void* memmove(void* destination, const void* source, size_t num);


// Compare two blocks of memory
int memcmp(const void* ptr1, const void* ptr2, size_t num);


// Search for a byte in a block of memory
void* memchr(const void* ptr, int value, size_t num);


// Securely set a block of memory to a specified value.
errno_t memset_s(void* ptr, size_t destsz, int value, size_t num);


```

#### BSD Unix System

```c

#include <strings.h>

// This function was used to zero out a block of memory
void bzero(void* ptr, size_t num);


// This function was used to copy a block of memory from one location to another
void bcopy(const void *src, void *dest, size_t n);


// This function was used to compare two blocks of memory
int bcmp(const void *s1, const void *s2, size_t n);


// This function was used to set a block of memory to a specified byte
void bcmp(void *s, int c, size_t n);


```

## `<sys/socket.h>`

### Types

```c

#define sa_family_t unsigned short;

```

### Generic Socket Structure

```c

#include <sys/socket.h>


struct sockaddr {
	sa_family_t   sa_family;    // Address family (e.g., AF_INET, AF_INET6)
	char          sa_data[14];  // Address data (varies on the address family)
}

```

### System Calls

#### `socket()`

```c
int socket(int domain, int type, int protocol);
```

- It returns -1 if the process is failed
- **Domain**: Network Layer
  - AF_UNIX --> UNIX domain socket
  - AF_LOCAL --> Local socket using UNIX file system for IPC
  - AF_INET --> Internet domain socket for IPv4
  - AF_INET6 --> Internet domain socket for IPv6
  - AF_X25 --> Packet-switched networking protocol, used in WAN
  - AF_APPLETLK --> AppleTalk protocol, used in Apple's older Macintosh
- **Type**: Transport Layer
  - SOCK_STREM --> Byte-stream socket for TCP
  - SOCK_DGRAM --> Raw Protocol Interface for UDP
  - SOCK_RAW --> Raw Protocol Interface
  - SOCK_SEQPACKET --> Sequenced-packet socket
- **Protocol**: Transport Layer
  - 0 --> the system to choose the appropriate protocol
  - IPPROTO_IP --> for AF_UNIX domain
  - IPPROTO_TCP --> for TCP sockets
  - IPPROTO_UDP --> for UDP sockets

#### `bind()`

```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

#### `listen()`

```c
int listen(int sockfd, int backlog);
```

#### `connect()`

```c
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

#### `accept()`

```c
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

#### `send()`

```c
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

#### `recv()`

```c
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

#### `sendto()`

```c
ssize_t recv(int sockfd, void *buf, size_t len, int flags, sockaddr *dest_addr, socklen_t* addrlen);
```

`

#### `recvfrom()`

```c
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
```

## `<netinet/in.h>`

### Types

```c

#define in_port_t uint16_t
#define in_addr_t uint32_t

```

### Internet Socket Structure

#### IPv4

```c

#include <sys/socket.h>
#include <netinet/in.h>


struct in_addr {
    in_addr_t        s_addr;       // IPv4 address (in network byte order)
};


struct sockaddr_in {
    sa_family_t      sin_family;   // Address family, AF_INET
    struct in_addr   sin_addr;     // IPv4 address
    in_port_t        sin_port;     // Port number (in network byte order)
    char             sin_zero[8];  // Unused, set to zero
};

```

- **Why `s_addr` is declare in a different structure?**

  The separation of the IPv4 address (`s_addr`) into its own structure (`struct in_addr`) allows for better organization and encapsulation. It also enables the reuse of the `struct in_addr` structure in other parts of the networking code where only the address is needed, without needing to include the port or other socket-related information.

- **Typecasting from Internet Socket `sockaddr_in` to Generic Socket `sockaddr *`**

  In C, when you cast a pointer from one struct type to another, as in the case of `struct sockaddr_in` to `struct sockaddr`, the compiler doesn't actually convert or modify the data. It simply interprets the data at that memory location according to the new type.

  The reason this works in the context of socket programming is because `struct sockaddr_in` is designed to be a superset of `struct sockaddr`. This means that the initial fields in `struct sockaddr_in` match exactly the fields in `struct sockaddr`. When you cast a pointer from `struct sockaddr_in *` to `struct sockaddr *`, you're essentially telling the compiler, "Trust me, this pointer actually points to a `struct sockaddr_in` object, but treat it as if it's a `struct sockaddr` object."

  This approach is used to maintain compatibility with older socket APIs that expect a `struct sockaddr *` but can handle `struct sockaddr_in` data correctly because of the way the structures are designed.

#### IPv6

```c

#include <sys/socket.h>
#include <netinet/in.h>


struct in6_addr {
    uint8_t          s6_addr[16];       // IPv6 (16 bytes in network byte order)
};


struct sockaddr_in6 {
    sa_family_t      sin6_family;      // Address family, AF_INET6
    struct in6_addr  sin6_addr;        // IPv6 address
    in_port_t        sin6_port;        // Port number (in network byte order)
    uint32_t         sin6_flowinfo;    // IPv6 traffic class and flow information
    uint32_t         sin6_scope_id;    // Scope ID (interface index)
};

```

### Byte Ordering Functions

```c

#include <netinet/in.h>


// Host to network short (16-bit) conversion
uint16_t htons(uint16_t hostshort);

// Host to network long (32-bit) conversion
uint32_t htonl(uint32_t hostlong);




// Network to host short (16-bit) conversion
uint16_t ntohs(uint16_t netshort);

// Network to host long (32-bit) conversion
uint32_t ntohl(uint32_t netlong);


```

Reference: [1](https://www.gnu.org/software/libc/manual/html_node/Byte-Order.html) | [2](https://www.gta.ufrj.br/ensino/eel878/sockets/htonsman.html)

## `<arpa/inet.h>`

### IP Address Conversion Functions

Convert IPv4 Address:

```c

#include <arpa/inet.h>


// String --> Binary
in_addr_t inet_addr(const char* cp);
// It returns INADDR_NONE if the conversion fails



// String --> Binary
int inet_aton(const char* cp, struct in_addr* inp);
// It returns 1 if the conversion is successful and 0 if it fails



// Binary --> String
char* inet_ntoa(struct in_addr in);


```

Convert IPv4 or IPv6 Address:

```c

#include <arpa/inet.h>


// Binary --> String(Presentaion)
const char* inet_ntop(int af, const void* src, char* dst, socklen_t size);


// String(Presentation) --> Binary
int inet_pton(int af, const char* src, void* dst);

```

## `<sys/types.h>`

```c

id_t
uid_t
pid_t


pthread_t
pthread_key_t
pthread_mutex_t


size_t
ssize_t


time_t


```
