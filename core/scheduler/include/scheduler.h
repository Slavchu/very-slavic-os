#pragma once
#include <hal/context_operations.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*task_entry_point_t)();

enum task_state : uint8_t {
    TASK_STATE_STOP,
    TASK_STATE_WAITING_FOR_RUN,
    TASK_STATE_RUNNING,
    TASK_STATE_BLOCKED,
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
    uint32_t time_delay_ms;
    uint16_t blocker_id;
    uint16_t task_id;
    enum task_state task_state;
    enum task_priority task_priority;
};

void scheduler_init(task_entry_point_t entry_point);

void scheduler_start();

void scheduler_tick(void *priv);

uint16_t scheduler_get_delay();

struct scheduler_task_ctx *scheduler_create_task(task_entry_point_t entry_point, enum task_priority prio);

void scheduler_set_current_task_state(enum task_state state, uint16_t blocker_id);

uint16_t scheduler_register_blocker();

bool scheduler_unblock_task_by_blocker_id(uint16_t blocker_id);

uint16_t get_current_task_id();

void sleep(uint32_t time_ms);
