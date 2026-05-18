#pragma once
#include <bitops.h>
#include <esp32c3/esp32c3_periph_gpio.h>
#include <esp32c3/esp32c3_periph_interrupt.h>
#include <esp32c3/esp32c3_periph_systimer.h>
#include <stdint.h>

/* Defined in esp32c3.peripheral.ld */
/* Sincerely, very-slavic-os developer */
extern volatile struct esp32c3_systimer SYSTIMER;
extern volatile struct esp32c3_interrupt_matrix INTERRUPT;
extern volatile struct esp32c3_gpio GPIO;

extern volatile uint32_t UART0[];
extern volatile uint32_t UART1[];
extern volatile uint32_t SPIMEM1[];
extern volatile uint32_t SPIMEM0[];
extern volatile uint32_t SDM[];
extern volatile uint32_t RTCCNTL[];
extern volatile uint32_t RTCIO[];
extern volatile uint32_t EFUSE[];
extern volatile uint32_t HINF[];
extern volatile uint32_t I2S0[];
extern volatile uint32_t I2C0[];
extern volatile uint32_t UHCI0[];
extern volatile uint32_t HOST[];
extern volatile uint32_t RMT[];
extern volatile uint32_t RMTMEM[];
extern volatile uint32_t SLC[];
extern volatile uint32_t LEDC[];
extern volatile uint32_t TIMERG0[];
extern volatile uint32_t TIMERG1[];
extern volatile uint32_t GPSPI2[];
extern volatile uint32_t SYSCON[];
extern volatile uint32_t TWAI[];
extern volatile uint32_t APB_SARADC[];
extern volatile uint32_t USB_SERIAL_JTAG[];
extern volatile uint32_t GDMA[];
extern volatile uint32_t SYSTEM[];

/* System registers*/
#define SYSTEM_PERIP_CLK_EN0_REG 0x10
#define SYSTEM_SYSTIMER_CLK_EN_BIT BIT(29)

#define SYSTEM_PERIP_RST_EN0_REG 0x18
