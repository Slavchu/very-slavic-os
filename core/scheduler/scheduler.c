#include <hal/interrupt.h>
#include <log.h>
#include <scheduler.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_TASKS
#define MAX_TASKS 32
#endif

#ifndef STACK_SIZE
#define STACK_SIZE 4096
#endif

extern uintptr_t _stack_top;
extern uintptr_t _stack_bot;

static struct {
    struct scheduler_task_ctx tasks[MAX_TASKS];
    struct scheduler_task_ctx *current_task;
    uint32_t frame_size_ms;
} scheduler_ctx;

static void update_task_queue();

static void task_runner() {
    scheduler_ctx.current_task->entry_point();

    interrupt_disable_isr();
    scheduler_ctx.current_task->task_state = TASK_STATE_STOP;
    free(scheduler_ctx.current_task->stack);
    interrupt_enable_isr();
    while (1) {
        ;
    }
}

void scheduler_init(task_entry_point_t entry_point) {
    memset(scheduler_ctx.tasks, 0, sizeof(scheduler_ctx.tasks));
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        scheduler_ctx.tasks[i].task_state = TASK_STATE_STOP;
    }

    scheduler_ctx.current_task = &scheduler_ctx.tasks[0];
    scheduler_ctx.current_task->task_priority = TASK_PRIORITY_LOW;
    scheduler_ctx.current_task->task_state = TASK_STATE_WAITING_FOR_RUN;
    scheduler_ctx.current_task->stack_size = (_stack_top - _stack_bot);
    scheduler_ctx.current_task->stack = (uint8_t *)_stack_top;
    scheduler_ctx.current_task->entry_point = entry_point;
    scheduler_ctx.current_task->ctx = hal_context_operations_init((uint8_t *)_stack_top, entry_point);
    update_task_queue();
    entry_point();
}

struct scheduler_task_ctx *scheduler_create_task(task_entry_point_t entry_point, enum task_priority prio) {
    interrupt_disable_isr();
    struct scheduler_task_ctx *task_ctx = NULL;
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        if (scheduler_ctx.tasks[i].task_state == TASK_STATE_STOP) {
            task_ctx = &scheduler_ctx.tasks[i];
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

    interrupt_enable_isr();
    return task_ctx;
}

static struct scheduler_task_ctx *task_queue[MAX_TASKS] = {0};
static uint8_t task_queue_idx = 0;

static void update_task_queue() {
    task_queue_idx = 0;
    for (uint8_t i = 0; i < MAX_TASKS; i++) {
        if (scheduler_ctx.tasks[i].task_state == TASK_STATE_WAITING_FOR_RUN) {
            task_queue[task_queue_idx++] = &scheduler_ctx.tasks[i];
        }
    }
    if (task_queue_idx < MAX_TASKS) {
        task_queue[task_queue_idx] = NULL;
    }
    task_queue_idx = 0;
}

static struct scheduler_task_ctx *task_queue_pop() {
    if (task_queue_idx == MAX_TASKS || task_queue[task_queue_idx] == NULL) {
        return NULL;
    }
    return task_queue[task_queue_idx++];
}

void scheduler_tick(hal_task_context *ctx) {
    scheduler_ctx.current_task->ctx = *ctx;
    scheduler_ctx.current_task->task_state = TASK_STATE_WAITING_FOR_RUN;

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

    *ctx = next_task->ctx;
}
