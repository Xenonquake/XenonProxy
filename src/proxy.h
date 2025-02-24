#ifndef XENONPROXY_PROXY_H
#define XENONPROXY_PROXY_H

#include <stdint.h>
#include <stdbool.h>
#include "../include/config.h"

// Error codes
typedef enum {
    PROXY_OK = 0,
    PROXY_ERROR_SOCKET = -1,
    PROXY_ERROR_BIND = -2,
    PROXY_ERROR_LISTEN = -3,
    // Add more error codes as needed
} proxy_error_t;

// Function declarations
int proxy_start(void);
void proxy_stop(void);

#endif // XENONPROXY_PROXY_H
