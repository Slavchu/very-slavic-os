#include <esp32c3/esp32c3_common.h>
#include <esp32c3/esp32c3_periph.h>
#include <hal/hal.h>
#include <hal/interrupt.h>
#include <hal/systimer.h>
#include <scheduler.h>
#include <utils.h>

/*
 * The systimer is incremented on 1/16 of us. This is extremily low.
 * I need a 1 ms precision
 */
#define TICKS_PER_US 16
#define SYSTIMER_TIMER_PERIOD_MS 1
#define SYSTIMER_TIMER_PERIOD_TICKS SYSTIMER_TIMER_PERIOD_MS * 1000 * TICKS_PER_US

#ifndef SYSTIMERS_MAX_ALARMS
#define SYSTIMER_MAX_ALARMS 8
#endif

struct hal_systimer_alarm {
    hal_alarm_cb_t cb;
    void *priv;
    uint8_t period_ms;
    uint8_t time_left_ms;
    bool is_enable;
    bool is_periodic;
};

volatile struct hal_systimer_alarm alarms[SYSTIMER_MAX_ALARMS] = {0};

hal_alarm_id_t hal_systimer_alarm_set(uint32_t delay_ms, hal_alarm_cb_t cb, bool is_periodic, void *priv) {
    hal_alarm_id_t res = -1;
    bool isr_status = interrupt_disable_isr();

    if (!delay_ms || !cb) {
        res = -1;
        goto end;
    }

    for (unsigned i = 0; i < SYSTIMER_MAX_ALARMS; ++i) {
        if (!alarms[i].is_enable) {
            alarms[i].cb = cb;
            alarms[i].time_left_ms = delay_ms;
            alarms[i].is_periodic = is_periodic;
            if (is_periodic) {
                alarms[i].period_ms = delay_ms;
            }
            alarms[i].is_enable = true;
            res = i;
            goto end;
        }
    }

end:
    if (isr_status) {
        interrupt_enable_isr();
    }
    return res;
}

void hal_systimer_set_alarm_delay(hal_alarm_id_t alarm_id, uint32_t delay_ms) {
    if (alarm_id < 0 || alarm_id >= SYSTIMER_MAX_ALARMS) {
        return;
    }
    alarms[alarm_id].period_ms = delay_ms;
    alarms[alarm_id].time_left_ms = delay_ms;
}

USED static void systimer_interrupt() {
    SYSTIMER.int_clr = 1;
    hal_clear_system_interrupt(SYSTIMER_INTERRUPT_ID);

    for (unsigned i = 0; i < SYSTIMER_MAX_ALARMS; ++i) {
        if (alarms[i].is_enable && !--alarms[i].time_left_ms) {
            alarms[i].cb(alarms[i].priv);
            if (alarms[i].is_periodic) {
                alarms[i].time_left_ms = alarms[i].period_ms;
                continue;
            }
            alarms[i].is_enable = false;
        }
    }
}

void hal_systimer_init() {
    SYSTEM[SYSTEM_PERIP_CLK_EN0_REG / sizeof(SYSTEM[0])] |= SYSTEM_SYSTIMER_CLK_EN_BIT;

    SYSTIMER.target0_conf.val = 0;
    SYSTIMER.target0_conf.period_mode = 1;
    SYSTIMER.target0_conf.period = (SYSTIMER_TIMER_PERIOD_TICKS & 0x1FFFFFF);

    SYSTIMER.target0_lo = 0;
    SYSTIMER.target0_hi.val = 0;
    SYSTIMER.target0_load = 1;
    SYSTIMER.int_ena = 1;
    SYSTIMER.conf.val = 0;
    SYSTIMER.conf.unit0_work_en = 1;
    SYSTIMER.conf.target0_work_en = 1;

    INTERRUPT.systimer_target0_map = SYSTIMER_INTERRUPT_ID;
    INTERRUPT.pri[SYSTIMER_INTERRUPT_ID] = INTERRUPT_PRIORITY_MAX;
    INTERRUPT.int_type &= ~BIT(SYSTIMER_INTERRUPT_ID);
    INTERRUPT.int_enable |= BIT(SYSTIMER_INTERRUPT_ID);

    interrupt_register(SYSTIMER_INTERRUPT_ID, systimer_interrupt, true);
}

uint64_t hal_systimer_get_sys_time_ms() {
    SYSTIMER.unit0_op_reg.update = 1;
    while (!SYSTIMER.unit0_op_reg.value_valid) {
        ;
    }

    uint64_t value = (uint64_t)(SYSTIMER.unit0_val_hi.value_hi) << 32 | SYSTIMER.unit0_val_lo;
    // Approximation of 16000. The divider is 16384;
    return value >> 14;
}
