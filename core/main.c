#include <ets_sys.h>
#include <func_table.h>
#include <hal/hal.h>
#include <hal/watchdog.h>
#include <stdint.h>
#include <systimer.h>

void init() { run_table(FUNC_TABLE_PREINIT, 0); }

void main() {
    init();

    for (int i = 0; i < 1000000; i++)
        ;

    while (1) {
        if (!flag)
            ets_printf("Zalupa\r\n");
        else {
            flag = 0;
            ets_printf("Interrupted\r\n");
        }
        for (int i = 0; i < 1000000; i++)
            ;
    }
}
