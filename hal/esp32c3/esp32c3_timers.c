#include <esp32c3/esp32c3_common.h>
#include <esp32c3/esp32c3_periph.h>
#include <hal/hal.h>
#include <hal/interrupt.h>
#include <scheduler.h>
#include <systimer.h>

#define DEFAULT_SYSTIMER_PERIOD 400000000

static void systimer_interrupt(hal_task_context *ctx) {
    hal_clear_systimer_interrupt();
    // TODO: Refactor by adding callbacks
    scheduler_tick(ctx);
}

void hal_setup_systimer(void) {
    SYSTEM[SYSTEM_PERIP_CLK_EN0_REG / sizeof(SYSTEM[0])] |= SYSTEM_SYSTIMER_CLK_EN_BIT;

    SYSTIMER.target0_conf.val = 0;
    SYSTIMER.target0_conf.period_mode = 1;
    SYSTIMER.target0_conf.period = (DEFAULT_SYSTIMER_PERIOD & 0x1FFFFFF);

    SYSTIMER.target0_lo = 0;
    SYSTIMER.target0_hi.val = 0;
    SYSTIMER.target0_load = 1;
    SYSTIMER.int_ena = 1;
    SYSTIMER.conf.val = 0;
    SYSTIMER.conf.unit0_work_en = 1;
    SYSTIMER.conf.target0_work_en = 1;

    INTERRUPT.systimer_target0_map = 7;
    INTERRUPT.pri[7] = INTERRUPT_PRIORITY_MAX;
    INTERRUPT.int_type &= ~BIT(7);
    INTERRUPT.int_enable |= BIT(7);

    interrupt_register(7, systimer_interrupt, true);
}

void hal_clear_systimer_interrupt(void) {
    SYSTIMER.int_clr = 1;
    INTERRUPT.int_clear = BIT(7);
}
