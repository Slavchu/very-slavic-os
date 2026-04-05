#include <hal/context_operations.h>
#include <hal/hal.h>
#include <limits.h>
#include <log.h>
#include <stdint.h>
#include <systimer.h>
#include <task.h>
#include <utils.h>

#define CLINT_BASE 0x600C1000
#define CLINT_MTIME_LOW (*(volatile uint32_t *)(CLINT_BASE + 0x0000))
#define CLINT_MTIME_HIGH (*(volatile uint32_t *)(CLINT_BASE + 0x0004))
#define CLINT_MTIMECMP_LOW (*(volatile uint32_t *)(CLINT_BASE + 0x0008))
#define CLINT_MTIMECMP_HIGH (*(volatile uint32_t *)(CLINT_BASE + 0x000C))
#define TICKS_PER_MS 16000

void riscv_interrupts_clint(hal_task_context ctx) {
    hal_clear_systimer_interrupt();
    systimer_tick();

    struct task_ctx *current = get_current_task();
    current->ctx = ctx;
    switch_task(ctx);
}

void riscv_interrupts_panic() {
    uint32_t mepc, mtval, mcause;
    __asm__ volatile("csrr %0, mepc\n"
                     "csrr %1, mtval\n"
                     "csrr %2, mcause\n"
                     : "=r"(mepc), "=r"(mtval), "=r"(mcause));

    LOG_CRITICAL("=== SLAVIC KERNEL BLYATED ===");
    LOG_CRITICAL("MCAUSE : %d (0x%08x)\n", mcause, mcause);
    LOG_CRITICAL("MEPC   : 0x%08x \n", mepc);
    LOG_CRITICAL("MTVAL  : 0x%08x (Опкод, що викликав паніку)\n", mtval);
    LOG_CRITICAL("===========================\n");
    hal_reboot();
}
