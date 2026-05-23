#include <hal/context_operations.h>

volatile hal_task_context current_ctx;

void set_current_context(volatile hal_task_context ctx) {
    current_ctx = ctx;
}

volatile hal_task_context get_current_context() {
    return current_ctx;
}
