#include <esp32c3/esp32c3_periph.h>
#include <hal/hal.h>
#include <hal/interrupt.h>

extern void ets_install_uart_printf(void);

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
