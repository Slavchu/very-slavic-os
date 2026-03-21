#include <hal/hal.h>

extern void ets_install_uart_printf(void);

void hal_init(void *UNUSED) {
    ets_install_uart_printf(); 
}
