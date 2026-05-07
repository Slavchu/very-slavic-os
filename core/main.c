#include <ets_sys.h>
#include <func_table.h>
#include <hal/hal.h>
#include <hal/interrupt.h>
#include <hal/watchdog.h>
#include <log.h>
#include <scheduler.h>
#include <syscall.h>

void task1() {
    while (1) {
        for (int i = 0; i < 1000000; i++) {
            ;
        }
        LOG_INFO("ZALUPA 1\r\n");
    }
}

void task2() {
    while (1) {
        for (int i = 0; i < 1000000; i++) {
            ;
        }
        LOG_INFO("ZALUPA 2\r\n");
        syscall_invoke(SYSCALL_REASON_YIELD);
        LOG_INFO("ZALUPA 2,2\r\n");
    }
}

void os_main() {

    LOG_INFO("==========SLAVIC OS BOOTED==========\r\n");
    scheduler_create_task(task1, TASK_PRIORITY_LOW);
    scheduler_create_task(task2, TASK_PRIORITY_HIGH);
    interrupt_enable_isr();
    while (1) {
        for (int i = 0; i < 1000000; i++) {
            ;
        }
        LOG_INFO("ZALUPA 3\r\n");
    }
}

void main() {
    run_table(FUNC_TABLE_PREINIT, 0);
    scheduler_init(os_main);

    while (1)
        ;
}
