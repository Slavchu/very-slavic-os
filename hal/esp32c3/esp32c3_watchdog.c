#include <esp32c3/esp32c3_timers.h>
#include <esp32c3/esp32c3_watchdog.h>
#include <hal/watchdog.h>

struct watchdog_ctx watchdogs[] = {
    {
        .wdt_specific_ctx.esp32c3_ctx.wdt_type = WDT_TYPE_ESP32C3_WDT_TG0,
        .wdt_driver = WDT_DRIVER_ESP32C3,
    },
    {
        .wdt_specific_ctx.esp32c3_ctx.wdt_type = WDT_TYPE_ESP32C3_WDT_TG1,
        .wdt_driver = WDT_DRIVER_ESP32C3,
    },
    {
        .wdt_specific_ctx.esp32c3_ctx.wdt_type = WDT_TYPE_ESP32C3_WDT_RTC,
        .wdt_driver = WDT_DRIVER_ESP32C3,
    },
};

const size_t watchdog_cnt = sizeof(watchdogs) / sizeof(struct watchdog_ctx);

uint8_t watchdog_init(struct watchdog_ctx *ctx, struct watchdog_params params) {
    ctx->params = params;
    return 0;
}

void watchdog_set_enable(struct watchdog_ctx *ctx, uint8_t enable) {
    ctx->params.enable = enable;

    uint32_t base_addr = 0;
    uint32_t config_reg_offset = 0;
    uint32_t protect_reg_offset = 0;

    switch (ctx->wdt_specific_ctx.esp32c3_ctx.wdt_type) {
    case WDT_TYPE_ESP32C3_WDT_TG0:
        base_addr = ESP32C3_TIMERGROUP0;
        config_reg_offset = ESP32C3_TIMG_WDTCONFIG0_REG;
        protect_reg_offset = ESP32C3_TIMG_WDTWPROTECT_REG;
        break;

    case WDT_TYPE_ESP32C3_WDT_TG1:
        base_addr = ESP32C3_TIMERGROUP1;
        config_reg_offset = ESP32C3_TIMG_WDTCONFIG0_REG;
        protect_reg_offset = ESP32C3_TIMG_WDTWPROTECT_REG;
        break;

    case WDT_TYPE_ESP32C3_WDT_RTC:
        base_addr = ESP32C3_RTCCNTL;
        config_reg_offset = ESP32C3_RTC_WDTCONFIG0_REG;
        protect_reg_offset = ESP32C3_RTC_WDTWPROTECT_REG;
        break;

    default:
        return;
    }

    *REG_32(base_addr + protect_reg_offset) = ESP32C3_WDT_WPROTECT_KEY;

    if (enable) {
        *REG_32(base_addr + config_reg_offset) |= ESP32C3_WDT_EN_BIT;
    } else {
        *REG_32(base_addr + config_reg_offset) &= ~ESP32C3_WDT_EN_BIT;

        if (ctx->wdt_specific_ctx.esp32c3_ctx.wdt_type == WDT_TYPE_ESP32C3_WDT_RTC) {
            *REG_32(base_addr + config_reg_offset) &= ~ESP32C3_RTC_WDT_FLASHBOOT_MOD_EN;
        } else {
            *REG_32(base_addr + config_reg_offset) &= ~ESP32C3_TIMG_WDT_FLASHBOOT_MOD_EN;
        }
    }

    *REG_32(base_addr + protect_reg_offset) = 0;
}

void watchdog_set_timeout(struct watchdog_ctx *ctx, uint16_t timeout_ms) {
    ctx->params.timeout_ms = timeout_ms;
}
