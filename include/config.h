#ifndef XENONPROXY_CONFIG_H
#define XENONPROXY_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

// Configuration structure
typedef struct {
    uint16_t listen_port;
    char *listen_address;
    size_t max_connections;
    bool use_encryption;
    // Add more configuration options as needed
} proxy_config_t;

// Function declarations
proxy_config_t *config_init(void);
void config_free(proxy_config_t *config);

#endif // XENONPROXY_CONFIG_H
