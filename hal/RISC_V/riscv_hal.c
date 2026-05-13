#include <hal/hal.h>
#include <hal/interrupt.h>
#include <hal/systimer.h>
#include <limits.h>
#include <riscv_hal.h>
#include <stdint.h>

// vector table of IRQ
extern void _vectors_start();

static void riscv_hal_init_irq_vector(void) {
    uintptr_t table_addr = (uintptr_t)_vectors_start;
    uintptr_t mtvec_val = table_addr | 0x1;
    __asm__ volatile("csrw mtvec, %0" ::"r"(mtvec_val));
}

static void riscv_hal_setup_irq(void) {
    riscv_hal_init_irq_vector();
    hal_systimer_init();
    interrupt_init();
}

void riscv_hal_init(void *UNUSED) {
    riscv_hal_setup_irq();
}

void hal_reboot(void) {
}

uint32_t hal_syscall_invoke(unsigned syscall) {
    __asm__ volatile("mv a7, %0" ::"r"(syscall));
    __asm__ volatile("ecall");
    auto ctx = get_current_context();
    return ctx->a0;
}

__attribute__((noreturn)) void _exit(int status) {
    while (1) {
        __asm__ volatile("wfi");
    }
}
