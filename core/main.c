#include <hal/watchdog.h>
#include <hal/hal.h>
#include <stdint.h>

extern int ets_printf(const char *fmt, ...);

void init() { 
    hal_init();
}

void main() {
    init();

    for (int i = 0; i < 1000000; i++)
        ;

    while (1) {
        ets_printf("Zalupa\r\n");
        for (int i = 0; i < 1000000; i++)
            ;
    }
}
