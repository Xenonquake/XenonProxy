#define _GNU_SOURCE
#include "network.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>

xenon_socket_t *network_socket_create(int domain, int type, int protocol) {
    xenon_socket_t *sock = calloc(1, sizeof(xenon_socket_t));
    if (sock == nullptr) {
        return nullptr;
    }

    // Create socket with close-on-exec
    sock->fd = socket(domain, type | SOCK_CLOEXEC, protocol);
    if (sock->fd < 0) {
        free(sock);
        return nullptr;
    }

    sock->domain = domain;
    sock->type = type;
    sock->protocol = protocol;

    // Set SO_REUSEADDR by default
    int reuse = 1;
    if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        network_socket_close(sock);
        return nullptr;
    }

    return sock;
}

void network_socket_close(xenon_socket_t *socket) {
    if (socket != nullptr) {
        if (socket->fd >= 0) {
            close(socket->fd);
        }
        free(socket);
    }
}

ssize_t network_read(xenon_socket_t *socket, void *buffer, size_t length) {
    if (socket == nullptr || socket->fd < 0) {
        errno = EBADF;
        return -1;
    }

    #ifdef HAS_TCP_ZEROCOPY
    // Use zero-copy reads when available
    return recv(socket->fd, buffer, length, MSG_ZEROCOPY);
    #else
    return read(socket->fd, buffer, length);
    #endif
}

ssize_t network_write(xenon_socket_t *socket, const void *buffer, size_t length) {
    if (socket == nullptr || socket->fd < 0) {
        errno = EBADF;
        return -1;
    }

    #ifdef HAS_TCP_ZEROCOPY
    // Use zero-copy writes when available
    return send(socket->fd, buffer, length, MSG_ZEROCOPY);
    #else
    return write(socket->fd, buffer, length);
    #endif
}

bool network_set_nonblocking(xenon_socket_t *socket) {
    if (socket == nullptr || socket->fd < 0) {
        return false;
    }

    int flags = fcntl(socket->fd, F_GETFL, 0);
    if (flags < 0) {
        return false;
    }

    return fcntl(socket->fd, F_SETFL, flags | O_NONBLOCK) == 0;
}

bool network_set_keepalive(xenon_socket_t *socket) {
    if (socket == nullptr || socket->fd < 0) {
        return false;
    }

    int keepalive = 1;
    int keepidle = 60;
    int keepintvl = 10;
    int keepcnt = 5;

    return setsockopt(socket->fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) == 0 &&
           setsockopt(socket->fd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle)) == 0 &&
           setsockopt(socket->fd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl)) == 0 &&
           setsockopt(socket->fd, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt)) == 0;
}

bool network_set_nodelay(xenon_socket_t *socket) {
    if (socket == nullptr || socket->fd < 0) {
        return false;
    }

    int nodelay = 1;
    return setsockopt(socket->fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay)) == 0;
}
