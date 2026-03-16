#pragma once
#include <stdint.h>
#include <stddef.h>

struct watchdog_esp32c3_ctx {
    enum : uint8_t {
        WDT_TYPE_ESP32C3_WDT_TG0,
        WDT_TYPE_ESP32C3_WDT_TG1,
        WDT_TYPE_ESP32C3_WDT_RTC,
        WDT_TYPE_ESP32C3_INVALID,
    } wdt_type;
} __attribute__((packed));

struct watchdog_params {
    uint16_t timeout_ms;
    uint8_t enable;
};

struct watchdog_ctx {
    struct watchdog_params params;
    union {
        struct watchdog_esp32c3_ctx esp32c3_ctx;
        uint8_t val;
    } wdt_specific_ctx;
    enum {
        WDT_DRIVER_ESP32C3,
        WDT_DRIVER_INVALID,
    } wdt_driver;
};

extern struct watchdog_ctx watchdogs[];
const extern size_t watchdog_cnt;

uint8_t watchdog_init(struct watchdog_ctx *ctx, struct watchdog_params params);

void watchdog_set_enable(struct watchdog_ctx *ctx, uint8_t enable);

void watchdog_set_timeout(struct watchdog_ctx *ctx, uint16_t timeout_ms);
