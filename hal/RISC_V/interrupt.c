#include <hal/context_operations.h>
#include <hal/hal.h>
#include <hal/interrupt.h>
#include <limits.h>
#include <log.h>
#include <stdint.h>
#include <utils.h>

volatile static bool isr_enable_state = false;

void riscv_interrupts_panic() {
    uint32_t mepc, mtval, mcause;
    __asm__ volatile("csrr %0, mepc\n"
                     "csrr %1, mtval\n"
                     "csrr %2, mcause\n"
                     : "=r"(mepc), "=r"(mtval), "=r"(mcause));

    LOG_CRITICAL("=== SLAVIC KERNEL BLYATED ===\r\n");
    LOG_CRITICAL("MCAUSE : %d (0x%08x)\r\n", mcause, mcause);
    LOG_CRITICAL("MEPC   : 0x%08x \n", mepc);
    LOG_CRITICAL("MTVAL  : 0x%08x \r\n", mtval);
    LOG_CRITICAL("===========================\n");
    hal_reboot();
}

uint16_t interrupt_get_id() {
    uint32_t mcause = 0;
    __asm__ volatile("csrr %0, mcause" : "=r"(mcause));
    return mcause & 0x1F; // Clearing is interrupt bit
}

void interrupt_enable_isr() {
    __asm__ volatile("csrs mstatus, %0" ::"r"(1 << 3));
    isr_enable_state = true;
}

bool interrupt_disable_isr() {
    bool current_state = isr_enable_state;
    __asm__ volatile("csrc mstatus, %0" ::"r"(1 << 3));
    isr_enable_state = false;
    return current_state;
}
