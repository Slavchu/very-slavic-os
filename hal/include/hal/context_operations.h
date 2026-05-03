#pragma once

struct arch_context_t;

typedef struct arch_context_t *hal_task_context;

hal_task_context hal_context_operations_init(void *stack_top, void *entry_point);

void set_current_context(hal_task_context ctx);

hal_task_context get_current_context();
