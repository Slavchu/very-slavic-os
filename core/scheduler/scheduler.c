#include <hal/interrupt.h>
#include <hal/systimer.h>
#include <log.h>
#include <scheduler.h>
#include <stdlib.h>
#include <string.h>
#include <syscall.h>

#ifndef MAX_TASKS
#define MAX_TASKS 32
#endif

#ifndef STACK_SIZE
#define STACK_SIZE 4096
#endif

#define TASK_PRIORITY_TIME_QUANT_MS 2

extern uintptr_t _stack_top;
extern uintptr_t _stack_bot;

static struct {
    struct scheduler_task_ctx tasks[MAX_TASKS];
    struct scheduler_task_ctx *current_task;
    hal_alarm_id_t alarm_id;
    uint32_t frame_size_ms;
    uint16_t next_scheduler_delay_ms;
} scheduler_ctx;

uint32_t update_task_queue();

uint32_t scheduler_yield_cb();

static void task_runner() {
    scheduler_ctx.current_task->entry_point();

    bool isr_status = interrupt_disable_isr();
    scheduler_ctx.current_task->task_state = TASK_STATE_STOP;
    free(scheduler_ctx.current_task->stack);
    if (isr_status) {
        interrupt_enable_isr();
    }
    while (1) {
        ;
    }
}

void scheduler_init(task_entry_point_t entry_point) {
    memset(scheduler_ctx.tasks, 0, sizeof(scheduler_ctx.tasks));
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        scheduler_ctx.tasks[i].task_state = TASK_STATE_STOP;
    }
    syscall_bind(SYSCALL_REASON_YIELD, scheduler_yield_cb);

    scheduler_ctx.current_task = &scheduler_ctx.tasks[0];
    scheduler_ctx.current_task->task_priority = TASK_PRIORITY_LOW;
    scheduler_ctx.current_task->task_state = TASK_STATE_WAITING_FOR_RUN;
    scheduler_ctx.current_task->stack_size = (_stack_top - _stack_bot);
    scheduler_ctx.current_task->stack = (uint8_t *)_stack_top;
    scheduler_ctx.current_task->entry_point = entry_point;
    scheduler_ctx.current_task->ctx = hal_context_operations_init((uint8_t *)_stack_top, entry_point);
    update_task_queue();
    scheduler_ctx.alarm_id = hal_systimer_alarm_set(scheduler_ctx.current_task->task_priority * TASK_PRIORITY_TIME_QUANT_MS, scheduler_tick, true, NULL);
    entry_point();
}

struct scheduler_task_ctx *scheduler_create_task(task_entry_point_t entry_point, enum task_priority prio) {
    bool isr_status = interrupt_disable_isr();
    struct scheduler_task_ctx *task_ctx = NULL;
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        if (scheduler_ctx.tasks[i].task_state == TASK_STATE_STOP) {
            task_ctx = &scheduler_ctx.tasks[i];
            task_ctx->task_id = i;
            break;
        }
    }
    if (!task_ctx) {
        return NULL;
    }

    task_ctx->stack_size = STACK_SIZE;
    task_ctx->stack = malloc(STACK_SIZE);
    if (!task_ctx->stack) {
        return NULL;
    }

    task_ctx->task_priority = prio;
    task_ctx->entry_point = entry_point;
    task_ctx->task_state = TASK_STATE_WAITING_FOR_RUN;
    task_ctx->ctx = hal_context_operations_init(task_ctx->stack + STACK_SIZE, task_runner);

    if (isr_status) {
        interrupt_enable_isr();
    }
    return task_ctx;
}

static struct scheduler_task_ctx *task_queue[MAX_TASKS] = {0};
static uint8_t task_queue_idx = 0;

uint16_t scheduler_get_delay() {
    return scheduler_ctx.next_scheduler_delay_ms;
};

uint32_t update_task_queue() {
    task_queue_idx = 0;
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        if (scheduler_ctx.tasks[i].task_state == TASK_STATE_WAITING_FOR_RUN) {
            task_queue[task_queue_idx++] = &scheduler_ctx.tasks[i];
        }
    }
    if (task_queue_idx < MAX_TASKS) {
        task_queue[task_queue_idx] = NULL;
    }
    uint32_t ret = task_queue_idx;
    task_queue_idx = 0;
    return ret;
}

static struct scheduler_task_ctx *task_queue_pop() {
    if (task_queue_idx == MAX_TASKS || task_queue[task_queue_idx] == NULL) {
        return NULL;
    }
    return task_queue[task_queue_idx++];
}

void scheduler_tick(void *priv UNUSED) {
    bool isr_state = interrupt_disable_isr();
    scheduler_ctx.current_task->ctx = get_current_context();
    if (scheduler_ctx.current_task->task_state == TASK_STATE_RUNNING) {
        scheduler_ctx.current_task->task_state = TASK_STATE_WAITING_FOR_RUN;
    }

    struct scheduler_task_ctx *next_task = task_queue_pop();
    if (!next_task) {
        update_task_queue();
        next_task = task_queue_pop();
    }

    if (!next_task) {
        next_task = scheduler_ctx.current_task;
    }

    next_task->task_state = TASK_STATE_RUNNING;
    scheduler_ctx.current_task = next_task;
    scheduler_ctx.next_scheduler_delay_ms = next_task->task_priority * TASK_PRIORITY_TIME_QUANT_MS;
    hal_systimer_set_alarm_delay(scheduler_ctx.alarm_id, scheduler_ctx.next_scheduler_delay_ms);
    set_current_context(next_task->ctx);
    if (isr_state) {
        interrupt_enable_isr();
    }
}

uint32_t scheduler_yield_cb() {
    scheduler_tick(NULL);
    return 0;
}

void scheduler_set_current_task_state(enum task_state state, uint16_t blocker_id) {
    bool isr_state = interrupt_disable_isr();
    scheduler_ctx.current_task->task_state = state;
    if (state == TASK_STATE_BLOCKED) {
        scheduler_ctx.current_task->blocker_id = blocker_id;
    }
    if (isr_state) {
        interrupt_enable_isr();
    }
}

uint16_t scheduler_register_blocker() {
    static uint16_t blocker_id = 1;
    if (!blocker_id) {
        blocker_id++;
    }
    return blocker_id++;
}

bool scheduler_unblock_task_by_blocker_id(uint16_t blocker_id) {
    bool isr_state = interrupt_disable_isr();
    bool ret = false;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (scheduler_ctx.tasks[i].blocker_id == blocker_id && scheduler_ctx.tasks[i].task_state == TASK_STATE_BLOCKED) {
            scheduler_ctx.tasks[i].blocker_id = 0;
            scheduler_ctx.tasks[i].task_state = TASK_STATE_WAITING_FOR_RUN;
            ret = true;
            break;
        }
    }
    if (isr_state) {
        interrupt_enable_isr();
    }
    return ret;
}

uint16_t get_current_task_id() {
    return scheduler_ctx.current_task->task_id;
}
