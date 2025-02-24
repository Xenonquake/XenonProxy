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

// C2x features: Static assertions can be used to ensure certain conditions at compile time
// For example, ensuring that the size of proxy_error_t is as expected.
_Static_assert(sizeof(proxy_error_t) <= sizeof(int), "proxy_error_t must fit within an int");

#endif // XENONPROXY_PROXY_H
