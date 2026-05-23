#include <hal/context_operations.h>
#include <hal/hal.h>
#include <hal/interrupt.h>
#include <log.h>
#include <riscv_hal.h>
#include <stdint.h>
#include <syscall.h>
#include <utils.h>

USED hal_task_context riscv_interrupts_panic(hal_task_context ctx) {
    set_current_context(ctx);
    uint32_t mepc, mtval, mcause;
    __asm__ volatile("csrr %0, mepc\n"
                     "csrr %1, mtval\n"
                     "csrr %2, mcause\n"
                     : "=r"(mepc), "=r"(mtval), "=r"(mcause));

    riscv_mcause_exception_t exception = mcause;
    switch (exception) {
    case MCAUSE_EXC_ECALL_FROM_U_MODE:
    case MCAUSE_EXC_ECALL_FROM_M_MODE:
        uint32_t syscall_reason = ctx->a7;
        ctx->mepc += 4;
        ctx->a0 = syscall_proceed(syscall_reason);
        return get_current_context();
    default:
        break;
    }

    LOG_CRITICAL("=== SLAVIC KERNEL BLYATED ===");
    LOG_CRITICAL("MCAUSE : %d (0x%08x)", mcause, mcause);
    LOG_CRITICAL("MEPC   : 0x%08x ", mepc);
    LOG_CRITICAL("MTVAL  : 0x%08x ", mtval);
    LOG_CRITICAL("===========================\n");
    hal_reboot();
    return get_current_context();
}

uint16_t interrupt_get_id() {
    uint32_t mcause = 0;
    __asm__ volatile("csrr %0, mcause" : "=r"(mcause));
    return mcause & 0x1F; // Clearing is interrupt bit
}

void interrupt_enable_isr() {
    __asm__ volatile("csrs mstatus, %0" ::"r"(1 << 3));
}

bool interrupt_disable_isr(void) {
    uint32_t old_mstatus;
    __asm__ volatile("csrrc %0, mstatus, %1" : "=r"(old_mstatus) : "r"(1 << 3) : "memory");

    return (old_mstatus & 1 << 3) != 0;
}
