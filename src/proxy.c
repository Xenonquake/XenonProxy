#define _GNU_SOURCE
#include "proxy.h"
#include "network.h"
#include <errno.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <sys/sendfile.h>
#include <stdatomic.h>

// Proxy context structure
typedef struct proxy_context {
    xenon_socket_t *listen_socket;
    int epoll_fd;
    proxy_config_t *config;
    volatile bool running;
    // Advanced memory pool for connections
    struct {
        void *pool;
        size_t size;
        _Atomic size_t used;
    } mem_pool;
} proxy_context_t;

// Thread-safe connection tracking
static _Atomic size_t active_connections = 0;
static proxy_context_t *global_context = nullptr;

// Initialize proxy with secure defaults
[[nodiscard]] int proxy_start(void) {
    // Enforce resource limits
    struct rlimit nofile = {.rlim_cur = 1024, .rlim_max = 4096};
    if (setrlimit(RLIMIT_NOFILE, &nofile) < 0) {
        return PROXY_ERROR_INIT;
    }

    // Initialize proxy context
    global_context = calloc(1, sizeof(proxy_context_t));
    if (!global_context) {
        return PROXY_ERROR_MEMORY;
    }

    // Create epoll instance with close-on-exec
    global_context->epoll_fd = epoll_create1(EPOLL_CLOEXEC);
    if (global_context->epoll_fd < 0) {
        goto cleanup;
    }

    // Initialize listening socket
    global_context->listen_socket = network_socket_create(
        AF_INET6,           // IPv6 (with IPv4 mapping)
        SOCK_STREAM,        // TCP
        IPPROTO_TCP
    );
    
    if (!global_context->listen_socket) {
        goto cleanup;
    }

    // Set socket options for security and performance
    network_set_nonblocking(global_context->listen_socket);
    network_set_keepalive(global_context->listen_socket);
    
    // Main event loop
    struct epoll_event events[64];
    global_context->running = true;
    
    while (global_context->running) {
        int nfds = epoll_wait(global_context->epoll_fd, events, 64, -1);
        if (nfds < 0 && errno != EINTR) {
            break;
        }
        
        for (int i = 0; i < nfds; i++) {
            // Handle events (implementation details in separate functions)
            handle_socket_event(&events[i]);
        }
    }

    return PROXY_OK;

cleanup:
    proxy_stop();
    return PROXY_ERROR_INIT;
}

// Clean shutdown with resource cleanup
void proxy_stop(void) {
    if (!global_context) {
        return;
    }

    global_context->running = false;

    if (global_context->listen_socket) {
        network_socket_close(global_context->listen_socket);
    }

    if (global_context->epoll_fd >= 0) {
        close(global_context->epoll_fd);
    }

    if (global_context->mem_pool.pool) {
        free(global_context->mem_pool.pool);
    }

    free(global_context);
    global_context = nullptr;
}
    free(global_context);
    global_context = nullptr;
}
