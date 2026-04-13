#include <ets_sys.h>
#include <hal/hal.h>
#include <hal/interrupt.h>
#include <limits.h>
#include <riscv_hal.h>
#include <riscv_interrupts.h>
#include <stdint.h>

#define RTC_CNTL_BASE 0x60008000
#define RTC_CNTL_OPTIONS0_REG (RTC_CNTL_BASE + 0x0000)
#define RTC_CNTL_SW_SYS_RST (1 << 31)

// vector table of IRQ
extern void _vectors_start();

static void riscv_hal_init_irq_vector(void) {
    uintptr_t table_addr = (uintptr_t)_vectors_start;
    uintptr_t mtvec_val = table_addr | 0x1;
    __asm__ volatile("csrw mtvec, %0" ::"r"(mtvec_val));
}

static void riscv_hal_setup_irq(void) {
    riscv_hal_init_irq_vector();
    hal_setup_systimer();
    interrupt_init();
    interrupt_enable_isr();
}

void riscv_hal_init(void *UNUSED) {
    riscv_hal_setup_irq();
}

void hal_reboot(void) {
    (*(volatile uint32_t *)RTC_CNTL_OPTIONS0_REG) |= RTC_CNTL_SW_SYS_RST;
}
