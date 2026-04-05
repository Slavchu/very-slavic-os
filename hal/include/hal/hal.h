#pragma once
#include <func_table.h>
#include <stddef.h>

void hal_init(void *UNUSED);
FUNC_TABLE_APPEND(FUNC_TABLE_PREINIT, hal_init);

void hal_reboot(void);

void hal_setup_systimer(void);

void hal_clear_systimer_interrupt(void);

void hal_switch_ctx(void *current_ctx, void *target_ctx, size_t ctx_size);
