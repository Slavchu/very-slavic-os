#pragma once

struct arch_context_t;

typedef struct arch_context_t *hal_task_context;

hal_task_context hal_context_operations_init(void *stack_top, void *entry_point);
