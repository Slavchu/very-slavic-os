#pragma once
#include <stddef.h>
#include <stdint.h>

struct uart_ctx;

struct uart_driver {
    void (*init)(struct uart_ctx *ctx);
    void (*deinit)(struct uart_ctx *ctx);
    void (*write)(struct uart_ctx *ctx, const char *buffer, size_t size);
    void (*read)(struct uart_ctx *ctx, const char *buffer, size_t sizinite);
};

struct uart_ctx {
    struct uart_driver driver;
    uint8_t uart_id;
    char *buf;
};
