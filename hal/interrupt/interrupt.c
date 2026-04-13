#include <hal/context_operations.h>
#include <hal/interrupt.h>
#include <log.h>
#include <utils.h>

#ifndef INTERRUPT_MAX
#define INTERRUPT_MAX 64
#endif

struct interrupt_param {
    interrupt_cb_t interrupt_handlers;
    bool enable;
};

static volatile struct interrupt_param interrupt_table[INTERRUPT_MAX] = {};

void interrupt_dispatcher(hal_task_context ctx) {
    auto interrupt_param = interrupt_table[interrupt_get_id()];

    if (!interrupt_param.interrupt_handlers || !interrupt_param.enable) {
        LOG_ERROR("Unhandled interrupt %d", interrupt_get_id());
        return;
    }
    interrupt_param.interrupt_handlers(ctx);
}

void interrupt_init() {
}

void interrupt_register(uint16_t interrupt_id, interrupt_cb_t interrupt, bool enable) {
    interrupt_table[interrupt_id].interrupt_handlers = interrupt;
    interrupt_table[interrupt_id].enable = enable;
}

void interrupt_enable(uint16_t interrupt_id) {
    interrupt_table[interrupt_id].enable = 1;
}

void interrupt_disable(uint16_t interrupt_id) {
    interrupt_table[interrupt_id].enable = 0;
}
