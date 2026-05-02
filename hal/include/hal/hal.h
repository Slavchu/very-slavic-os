#pragma once
#include <func_table.h>
#include <stddef.h>

void hal_init(void *UNUSED);
FUNC_TABLE_APPEND(FUNC_TABLE_PREINIT, hal_init);

void hal_reboot(void);

void hal_clear_system_interrupt(uint8_t interrupt_id);
