#include <ets_sys.h>
#include <func_table.h>
#include <hal/hal.h>
#include <hal/watchdog.h>
#include <log.h>
#include <stdint.h>
#include <systimer.h>
#include <task.h>

void task1(size_t argsize, uint8_t[argsize]) {
    while (1) {
        LOG_INFO("Zalupa 1\r\n");
        for (int i = 0; i < 1000000; i++)
            ;
    }
}

void task2(size_t argsize, uint8_t[argsize]) {
    while (1) {
        LOG_INFO("Zalupa 2\r\n");
        for (int i = 0; i < 1000000; i++)
            ;
    }
}
void init() {
    create_task(task1);
    create_task(task2);
    run_table(FUNC_TABLE_PREINIT, 0);
}

void main() {
    for (int i = 0; i < 1000000; i++)
        ;

    init();

    while (1) {
        LOG_INFO("Zalupa main\r\n");
        for (int i = 0; i < 1000000; i++)
            ;
    }
}
