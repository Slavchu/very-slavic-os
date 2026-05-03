#include <hal/context_operations.h>

hal_task_context current_ctx;

void set_current_context(hal_task_context ctx) {
    current_ctx = ctx;
}

hal_task_context get_current_context() {
    return current_ctx;
}
