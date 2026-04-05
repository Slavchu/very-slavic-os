#pragma once

struct arch_context_t;

typedef struct arch_context_t *hal_task_context;

void hal_context_operations_swap(hal_task_context ctx);

void hal_context_operations_init(hal_task_context *ctx_out, void *stack_top, void *entry_point);

void hal_context_operations_set_next_ctx(hal_task_context ctx);
