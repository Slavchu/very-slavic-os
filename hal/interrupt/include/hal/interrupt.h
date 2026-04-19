#pragma once

#include <hal/context_operations.h>
#include <stdint.h>

typedef void (*interrupt_cb_t)(hal_task_context *ctx);

void interrupt_init();

void interrupt_enable_isr();

void interrupt_disable_isr();

void interrupt_register(uint16_t interrupt_id, interrupt_cb_t handler, bool enable);

void interrupt_enable(uint16_t interrupt_id);

void interrupt_disable(uint16_t interrupt_id);

uint16_t interrupt_get_id();
