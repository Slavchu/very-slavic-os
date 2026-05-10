#include <arch_sync.h>
#include <hal/context_operations.h>
#include <hal/interrupt.h>
#include <mutex.h>
#include <scheduler.h>
#include <string.h>
#include <syscall.h>

void mutex_init(struct mutex *mutex) {
    mutex->blocker_id = scheduler_register_blocker();
    mutex->is_locked = false;
    mutex->locker = UINT16_MAX;
}

void mutex_deinit(struct mutex *mutex) {
    memset(mutex, 0, sizeof(struct mutex));
}

void mutex_lock(struct mutex *mutex) {
    while (true) {
        bool isr_status = interrupt_disable_isr();
        if (mutex->is_locked && mutex->locker != get_current_task_id()) {
            scheduler_set_current_task_state(TASK_STATE_BLOCKED, mutex->blocker_id);
        } else {
            mutex->locker = get_current_task_id();
            mutex->is_locked = true;
            smp_mb_acquire();
            if (isr_status) {
                interrupt_enable_isr();
            }
            return;
        }
        if (isr_status) {
            interrupt_enable_isr();
        }
        syscall_invoke(SYSCALL_REASON_YIELD);
    }
}

void mutex_unlock(struct mutex *mutex) {
    bool isr_status = interrupt_disable_isr();
    if (!mutex->is_locked) {
        goto end;
    }
    if (mutex->locker != get_current_task_id()) {
        goto end;
    }
    scheduler_unblock_task_by_blocker_id(mutex->blocker_id);
    smp_mb_release();
    mutex->is_locked = false;
    mutex->locker = UINT16_MAX;
end:
    if (isr_status) {
        interrupt_enable_isr();
    }
}
