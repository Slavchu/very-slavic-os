#pragma once
#include <stdint.h>

#define SYSTIMER_INTERRUPT_ID 7

typedef union {
    struct {
        uint32_t period : 26;
        uint32_t reserved26 : 4;
        uint32_t period_mode : 1;
        uint32_t timer_unit_sel : 1;
    };
    uint32_t val;
} systimer_target_conf_reg_t;

typedef union {
    struct {
        uint32_t val : 20;
        uint32_t reserved : 12;
    };
    uint32_t raw;
} systimer_target_hi_reg_t;

typedef union {
    struct {
        uint32_t reserved0 : 24;
        uint32_t target0_work_en : 1;
        uint32_t target1_work_en : 1;
        uint32_t target2_work_en : 1;
        uint32_t reserved27 : 3;
        uint32_t unit0_work_en : 1;
        uint32_t unit1_work_en : 1;
    };
    uint32_t val;
} systimer_conf_reg_t;

typedef union {
    struct {
        uint32_t value_hi : 20;
        uint32_t reserved : 12;
    };
    uint32_t val;
} systimer_unit_value_hi_reg_t;

typedef union {
    struct {
        uint32_t reserved1 : 29;
        uint8_t value_valid : 1;
        uint8_t update : 1;
        uint8_t reserved : 1;
    };
    uint32_t val;
} systimer_unit_op_reg;

struct esp32c3_systimer {
    volatile systimer_conf_reg_t conf;

    volatile systimer_unit_op_reg unit0_op_reg;
    volatile systimer_unit_op_reg unit1_op_reg;

    uint32_t res0[4];

    volatile systimer_target_hi_reg_t target0_hi;
    volatile uint32_t target0_lo;

    volatile uint32_t target1_hi;
    volatile uint32_t target1_lo;

    volatile uint32_t target2_hi;
    volatile uint32_t target2_lo;

    volatile systimer_target_conf_reg_t target0_conf;
    volatile systimer_target_conf_reg_t target1_conf;
    volatile systimer_target_conf_reg_t target2_conf;

    volatile systimer_unit_value_hi_reg_t unit0_val_hi;
    volatile uint32_t unit0_val_lo;
    volatile systimer_unit_value_hi_reg_t unit1_val_hi;
    volatile uint32_t unit1_val_lo;

    volatile uint32_t target0_load;
    volatile uint32_t target1_load;
    volatile uint32_t target2_load;

    uint32_t res2[2];

    volatile uint32_t int_ena;
    volatile uint32_t int_raw;
    volatile uint32_t int_clr;
};
