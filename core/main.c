#include <func_table.h>
#include <hal/hal.h>
#include <hal/interrupt.h>
#include <hal/watchdog.h>
#include <log.h>
#include <mutex.h>
#include <scheduler.h>
#include <syscall.h>

static struct mutex task_mtx;
void task1() {
    while (1) {
        sleep(10000);
        mutex_lock(&task_mtx);
        LOG_INFO("ZALUPA 1");
        mutex_unlock(&task_mtx);
    }
}

void task2() {
    while (1) {
        sleep(1000);
        mutex_lock(&task_mtx);
        LOG_INFO("ZALUPA 2");
        mutex_unlock(&task_mtx);
    }
}

void os_main() {

    printf("==========SLAVIC OS BOOTED==========\r\n");
    scheduler_create_task(task1, TASK_PRIORITY_HIGH);
    scheduler_create_task(task2, TASK_PRIORITY_HIGH);
    mutex_init(&task_mtx);
    interrupt_enable_isr();
    while (1) {
        sleep(5000);
        mutex_lock(&task_mtx);
        LOG_INFO("ZALUPA 3");
        mutex_unlock(&task_mtx);
    }
}

void main() {
    run_table(FUNC_TABLE_PREINIT, 0);
    scheduler_init(os_main);

    while (1)
        ;
}
