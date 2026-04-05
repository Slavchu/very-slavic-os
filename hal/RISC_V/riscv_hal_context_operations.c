#include <hal/context_operations.h>
#include <stdint.h>

struct arch_context_t {
    uint32_t ra, gp, tp, t0, t1, t2, s0, s1;
    uint32_t a0, a1, a2, a3, a4, a5, a6, a7;
    uint32_t s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    uint32_t t3, t4, t5, t6;
    uint32_t mepc, sp;
} PACKED;

volatile struct arch_context_t *next_ctx;

void hal_context_operations_init(hal_task_context *ctx_out, void *stack_top, void *entry_point) {
    uintptr_t sp = (uintptr_t)stack_top;
    sp = sp & ~0xF;

    sp = sp - sizeof(struct arch_context_t);
    struct arch_context_t *ctx = (struct arch_context_t *)sp;

    uint32_t *regs = (uint32_t *)ctx;
    for (int i = 0; i < sizeof(struct arch_context_t) / 4; i++) {
        regs[i] = 0;
    }

    ctx->mepc = (uint32_t)entry_point;
    ctx->sp = sp + sizeof(struct arch_context_t);

    *ctx_out = ctx;
}

void hal_context_operations_set_next_ctx(hal_task_context ctx) {
    next_ctx = ctx;
}
