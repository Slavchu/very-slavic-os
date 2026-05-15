#pragma once
#include <func_table.h>
#include <utils.h>

#define MIE_BIT_MACHINE_INTERRUPT_ENABLE 3
#define MIE_BIT_MACHINE_TIMER_INTERRUPT_ENABLE 7

typedef enum : unsigned {
    MCAUSE_EXC_INSTRUCTION_ADDRESS_MISALIGNED = 0,
    MCAUSE_EXC_INSTRUCTION_ACCESS_FAULT = 1,
    MCAUSE_EXC_ILLEGAL_INSTRUCTION = 2,
    MCAUSE_EXC_BREAKPOINT = 3,
    MCAUSE_EXC_LOAD_ADDRESS_MISALIGNED = 4,
    MCAUSE_EXC_LOAD_ACCESS_FAULT = 5,
    MCAUSE_EXC_STORE_ADDRESS_MISALIGNED = 6,
    MCAUSE_EXC_STORE_ACCESS_FAULT = 7,
    MCAUSE_EXC_ECALL_FROM_U_MODE = 8,
    MCAUSE_EXC_ECALL_FROM_M_MODE = 11,
    MCAUSE_EXC_INSTRUCTION_PAGE_FAULT = 12,
    MCAUSE_EXC_LOAD_PAGE_FAULT = 13,
    MCAUSE_EXC_STORE_PAGE_FAULT = 15
} riscv_mcause_exception_t;

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

void riscv_hal_init(void *UNUSED);
FUNC_TABLE_APPEND(FUNC_TABLE_PREINIT, riscv_hal_init);
