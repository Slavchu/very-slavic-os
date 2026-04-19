#include <hal/context_operations.h>
#include <hal/hal.h>
#include <hal/interrupt.h>
#include <limits.h>
#include <log.h>
#include <stdint.h>
#include <systimer.h>
#include <utils.h>

#define CLINT_BASE 0x600C1000
#define CLINT_MTIME_LOW (*(volatile uint32_t *)(CLINT_BASE + 0x0000))
#define CLINT_MTIME_HIGH (*(volatile uint32_t *)(CLINT_BASE + 0x0004))
#define CLINT_MTIMECMP_LOW (*(volatile uint32_t *)(CLINT_BASE + 0x0008))
#define CLINT_MTIMECMP_HIGH (*(volatile uint32_t *)(CLINT_BASE + 0x000C))
#define TICKS_PER_MS 16000

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
}

void interrupt_disable_isr() {
    __asm__ volatile("csrc mstatus, %0" ::"r"(1 << 3));
}
