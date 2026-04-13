#include <hal/hal.h>
#include <hal/interrupt.h>

extern void ets_install_uart_printf(void);

void hal_init(void *UNUSED) {
    ets_install_uart_printf();
    hal_setup_systimer();
}
