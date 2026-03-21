#include <riscv_registers.h>
#include <stdint.h>
#include <systimer.h>
#include <utils.h>
#include <hal/hal.h>
#include <ets_sys.h> //temporary for uart
#include <limits.h>

#define CLINT_BASE            0x600C1000
#define CLINT_MTIME_LOW       (*(volatile uint32_t*)(CLINT_BASE + 0x0000))
#define CLINT_MTIME_HIGH      (*(volatile uint32_t*)(CLINT_BASE + 0x0004))
#define CLINT_MTIMECMP_LOW    (*(volatile uint32_t*)(CLINT_BASE + 0x0008))
#define CLINT_MTIMECMP_HIGH   (*(volatile uint32_t*)(CLINT_BASE + 0x000C))
#define TICKS_PER_MS 16000

void riscv_interrupts_clint(struct riscv_register_ctx *ctx) { 
    hal_clear_systimer_interrupt();
    systimer_tick(); 
}

void riscv_interrupts_panic() {
    uint32_t mepc, mtval, mcause;
    __asm__ volatile (
    "csrr %0, mepc\n"
        "csrr %1, mtval\n"
        "csrr %2, mcause\n"
        : "=r"(mepc), "=r"(mtval), "=r"(mcause)
    );

    ets_printf("\n=== SLAVIC KERNEL PANIC ===\n");
    ets_printf("MCAUSE : %d (0x%08x)\n", mcause, mcause);
    ets_printf("MEPC   : 0x%08x (Адреса помилки)\n", mepc);
    ets_printf("MTVAL  : 0x%08x (Опкод, що викликав паніку)\n", mtval);
    ets_printf("===========================\n");
    __asm__ __volatile__ ("csrr %0, mcause" : "=r"(mcause));
    ets_printf("Panic occured. Reboot: mcause %d", mcause);
    uint32_t time = UINT32_MAX;
    while(time--)
        __asm__ __volatile__("nop");
    hal_reboot();
}
