#ifndef XENONPROXY_NETWORK_H
#define XENONPROXY_NETWORK_H

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Modern C23 features
#ifdef __STDC_VERSION__
    #if __STDC_VERSION__ >= 202311L
        #define NODISCARD [[nodiscard]]
    #else
        #define NODISCARD __attribute__((warn_unused_result))
    #endif
#endif

// Socket wrapper structure with RAII-style management
typedef struct xenon_socket {
    int fd;                  // File descriptor
    int domain;             // AF_INET, AF_INET6, etc.
    int type;               // SOCK_STREAM, SOCK_DGRAM
    int protocol;           // IPPROTO_TCP, IPPROTO_UDP
    struct sockaddr_storage addr;
    socklen_t addr_len;
} xenon_socket_t;

// Error handling with detailed context
typedef enum network_error {
    NET_OK = 0,
    NET_ERROR_SOCKET = -1,
    NET_ERROR_BIND = -2,
    NET_ERROR_LISTEN = -3,
    NET_ERROR_ACCEPT = -4,
    NET_ERROR_CONNECT = -5,
    NET_ERROR_SSL = -6,
    NET_ERROR_TIMEOUT = -7
} network_error_t;

// Function declarations with modern C attributes
NODISCARD xenon_socket_t *network_socket_create(int domain, int type, int protocol);
void network_socket_close(xenon_socket_t *socket);

// Non-blocking I/O operations
NODISCARD ssize_t network_read(xenon_socket_t *socket, void *buffer, size_t length);
NODISCARD ssize_t network_write(xenon_socket_t *socket, const void *buffer, size_t length);

// Event handling
typedef struct network_event {
    xenon_socket_t *socket;
    uint32_t events;        // EPOLLIN, EPOLLOUT, etc.
    void *user_data;
} network_event_t;

// Network utilities
NODISCARD bool network_set_nonblocking(xenon_socket_t *socket);
NODISCARD bool network_set_keepalive(xenon_socket_t *socket);
NODISCARD bool network_set_nodelay(xenon_socket_t *socket);

// Advanced feature detection
#if defined(__linux__) && defined(__GLIBC__)
    #include <linux/version.h>
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)
        #define HAS_TCP_ZEROCOPY
    #endif
#endif

#endif // XENONPROXY_NETWORK_H
