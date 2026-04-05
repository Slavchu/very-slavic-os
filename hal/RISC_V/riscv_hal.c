#include <ets_sys.h>
#include <hal/hal.h>
#include <limits.h>
#include <riscv_hal.h>
#include <stdint.h>

#define RTC_CNTL_BASE 0x60008000
#define RTC_CNTL_OPTIONS0_REG (RTC_CNTL_BASE + 0x0000)
#define RTC_CNTL_SW_SYS_RST (1 << 31)
#define CLINT_MTIMECMP_LOW (*(volatile uint32_t *)(0x600C1008))
#define CLINT_MTIMECMP_HIGH (*(volatile uint32_t *)(0x600C100C))
#define INTERRUPT_CORE0_CPU_INT_THRESH_REG (*(volatile uint32_t *)(0x600c2000))
// vector table of IRQ
extern void _vectors_start();

static void riscv_hal_init_irq_vector(void) {
    uintptr_t table_addr = (uintptr_t)_vectors_start;
    uintptr_t mtvec_val = table_addr | 0x1;
    __asm__ volatile("csrw mtvec, %0" ::"r"(mtvec_val));
}

static void riscv_hal_global_interrupts_enable() {
    __asm__ volatile("csrs mstatus, %0" ::"r"(1 << 3)); // csrs — set bits in CSR
}

static void riscv_hal_setup_irq(void) {
    riscv_hal_init_irq_vector();
    hal_setup_systimer();
    riscv_hal_global_interrupts_enable();
}

void riscv_hal_init(void *UNUSED) {
    riscv_hal_setup_irq();
}

void hal_reboot(void) {
    (*(volatile uint32_t *)RTC_CNTL_OPTIONS0_REG) |= RTC_CNTL_SW_SYS_RST;
}
