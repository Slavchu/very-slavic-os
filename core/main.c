#include <ets_sys.h>
#include <func_table.h>
#include <hal/hal.h>
#include <hal/watchdog.h>
#include <log.h>
#include <stdint.h>
#include <systimer.h>
#include <task.h>
#include <stdlib.h>

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
    int * rand_buf = malloc(50*sizeof(int));
    if(!rand_buf) {
        LOG_ERROR("Zalupa could not even allocate 50 elements via malloc\r\n");
        return;
    }
    rand_buf[24] = 25;
    LOG_INFO("%d\r\n", rand_buf[24]);
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
