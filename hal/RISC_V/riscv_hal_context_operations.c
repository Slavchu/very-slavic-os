#include <hal/context_operations.h>
#include <log.h>
#include <stdint.h>
#include <string.h>

struct arch_context_t {
    uint32_t ra;
    uint32_t gp;
    uint32_t tp;
    uint32_t t0, t1, t2;
    uint32_t s0, s1;
    uint32_t a0, a1, a2, a3, a4, a5, a6, a7;
    uint32_t s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    uint32_t t3, t4, t5, t6;
    uint32_t mepc;
    uint32_t sp;
} PACKED;

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
