#pragma once
#include <hal/context_operations.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*task_entry_point_t)();

enum task_state : uint8_t {
    TASK_STATE_STOP,
    TASK_STATE_WAITING_FOR_RUN,
    TASK_STATE_RUNNING,
    TASK_STATE_INVALID,
};

enum task_priority : uint8_t {
    TASK_PRIORITY_LOW = 1u,
    TASK_PRIORITY_NORMAL,
    TASK_PRIORITY_HIGH,
    TASK_PRIORITY_MAX_KEEP_MAX_AND_DO_NOT_USE_IT_IS_INVALID_PLEASE
};

struct scheduler_task_ctx {
    hal_task_context ctx;
    task_entry_point_t entry_point;
    size_t stack_size;
    uint8_t *stack;
    enum task_state task_state;
    enum task_priority task_priority;
};

void scheduler_init(task_entry_point_t entry_point);

void scheduler_start();

void scheduler_tick(hal_task_context *ctx);

struct scheduler_task_ctx *scheduler_create_task(task_entry_point_t entry_point, enum task_priority prio);
