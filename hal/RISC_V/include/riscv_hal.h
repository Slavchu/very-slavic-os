#pragma once
#include <func_table.h>

#define MIE_BIT_MACHINE_INTERRUPT_ENABLE 3
#define MIE_BIT_MACHINE_TIMER_INTERRUPT_ENABLE 7

void riscv_hal_init(void* UNUSED);
FUNC_TABLE_APPEND(FUNC_TABLE_PREINIT, riscv_hal_init);
