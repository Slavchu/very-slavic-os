#pragma once
#include <stdint.h>

struct esp32c3_gpio {
    volatile uint32_t bt_select_reg;  // 0x0000
    volatile uint32_t out;            // 0x0004
    volatile uint32_t out_w1ts;       // 0x0008
    volatile uint32_t out_w1tc;       // 0x000C
    volatile uint32_t reserved_10[4]; // 0x0010 ... 0x001C (Тут має бути 4 елементи!)
    volatile uint32_t enable;         // 0x0020
    volatile uint32_t enable_w1ts;    // 0x0024
    volatile uint32_t enable_w1tc;    // 0x0028
    volatile uint32_t reserved_2c[4]; // 0x002C ... 0x0038
    volatile uint32_t in;             // 0x003C
};

#define DR_REG_IO_MUX_BASE 0x60009000
#define REG_IO_MUX_GPIO(num) ((volatile uint32_t *)(DR_REG_IO_MUX_BASE + 0x04 + ((num) * 4)))
#define REG_GPIO_FUNC_OUT_SEL_CFG(num) ((volatile uint32_t *)(0x60004000 + 0x0554 + ((num) * 4)))

#define MCU_SEL_M (0x7 << 12)
#define FUN_IE (1 << 9)
#define FUN_WPU (1 << 8)
#define FUN_WPD (1 << 7)
