#include <esp32c3/esp32c3_periph.h>
#include <hal/hal.h>
#include <hal/interrupt.h>
#include <scheduler.h>
#include <stdio.h>

extern void ets_install_uart_printf(void);

/*This is a non-slavic code. I can't spend a lot of time to implement i/o, so gemini implemented it*/
#define UART_FIFO_REG 0x60000000
#define UART_STATUS_REG 0x6000001C
#define UART_RXFIFO_CNT_MASK 0x000003FF

static int uart_getc(FILE *file) {
    (void)file;

    while ((*((volatile uint32_t *)UART_STATUS_REG) & UART_RXFIFO_CNT_MASK) == 0) {
        sleep(1);
    }

    return (int)(*((volatile uint32_t *)UART_FIFO_REG) & 0xFF);
}

static int uart_putc(char c, FILE *file) {
    (void)file;
    extern void uart_tx_one_char(uint8_t c);
    uart_tx_one_char(c);
    return c;
}

static FILE __stdio = FDEV_SETUP_STREAM(uart_putc, uart_getc, NULL, _FDEV_SETUP_RW);

FILE *const stdin = &__stdio;
FILE *const stdout = &__stdio;
/* end of non-slavic code */

void hal_init(void *UNUSED) {
    ets_install_uart_printf();
}

void hal_clear_system_interrupt(uint8_t interrupt_id) {
    INTERRUPT.int_clear = BIT(interrupt_id);
}

void hal_idle_task() {
    while (1) {
        __asm__ volatile("wfi");
    }
}
