#include <hal/context_operations.h>
#include <log.h>
#include <riscv_hal.h>
#include <stdint.h>
#include <string.h>

hal_task_context hal_context_operations_init(void *stack_top, void *entry_point) {
    uintptr_t sp = (uintptr_t)stack_top;
    sp = sp & ~0xF;
    sp -= sizeof(struct arch_context_t);
    struct arch_context_t *ctx = (struct arch_context_t *)sp;
    memset(ctx, 0, sizeof(struct arch_context_t));
    ctx->mepc = (uint32_t)entry_point;
    ctx->sp = sp + sizeof(struct arch_context_t);

    register uint32_t current_gp __asm__("gp");
    ctx->gp = current_gp;
    return (hal_task_context)ctx;
}
