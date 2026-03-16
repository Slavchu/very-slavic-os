#include <hal/watchdog.h>
#include <stdint.h>

extern int ets_printf(const char *fmt, ...);

void disable_watchdogs() {
    for (int i = 0; i < watchdog_cnt; i++) {
        watchdog_set_enable(&watchdogs[i], 0);
    }
}

void init() { disable_watchdogs(); }

void main() {
    volatile unsigned int *uart0_fifo = (volatile unsigned int *)0x60000000;
    init();

    for (int i; i < 1000000; i++)
        ;

    while (1) {
        ets_printf("Zalupa\r\n");
        for (int i = 0; i < 1000000; i++)
            ;
    }
}
