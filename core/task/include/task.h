#pragma once
#include <hal/context_operations.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_TASKS 16

typedef void (*task_entry_point_t)(size_t argsize, uint8_t[argsize]);

enum task_state {
    TASK_STATE_STOP,
    TASK_STATE_WAITING_FOR_RUN,
    TASK_STATE_RUNNING,
    TASK_STATE_INVALID,
};

struct task_ctx {
    task_entry_point_t entry_point;
    uint8_t *stack;
    uint32_t stack_size;
    uint16_t task_id;
    enum task_state task_state;
    hal_task_context ctx;
};

struct task_ctx *create_task(task_entry_point_t entry_point);

void switch_task(hal_task_context current_sp);

struct task_ctx *get_current_task(void);
