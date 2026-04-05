#include <log.h>
#include <stddef.h>
#include <stdint.h>
#include <task.h>

#define STACK_SIZE 2048

static struct task_ctx tasks[MAX_TASKS] = {0};
static size_t task_count = 0;                  // How many tasks exist
static volatile size_t current_running_id = 0; // Which task is CURRENTLY running
static uint8_t stack[MAX_TASKS][STACK_SIZE];

struct task_ctx *create_task(task_entry_point_t entry_point) {
    if (task_count >= MAX_TASKS) {
        return NULL;
    }

    int id = task_count;
    struct task_ctx *task = &tasks[id];

    task->task_id = id;
    task->entry_point = entry_point;

    if (entry_point != NULL) {
        task->task_state = TASK_STATE_WAITING_FOR_RUN;
        hal_context_operations_init(&task->ctx, &stack[id][STACK_SIZE], entry_point);
    } else {
        task->task_state = TASK_STATE_RUNNING;
        task->ctx = NULL;
    }

    task_count++;
    return task;
}

struct task_ctx *get_current_task(void) {
    // Return the actually running task, not the total count!
    return &tasks[current_running_id];
}

void switch_task(hal_task_context current_sp) {
    // 1. Save the context into the task that was JUST running
    tasks[current_running_id].ctx = current_sp;

    // 2. Find the next task (Simple Round Robin)
    current_running_id++;
    if (current_running_id >= task_count) {
        current_running_id = 0;
    }

    LOG_INFO("TASK IDX %d\r\n", current_running_id);

    // 3. Update the global pointer for the assembly to pivot to
    hal_context_operations_set_next_ctx(tasks[current_running_id].ctx);
}
