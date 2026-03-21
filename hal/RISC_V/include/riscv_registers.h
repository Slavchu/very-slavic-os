#pragma once
#include <stdint.h>
#include <utils.h>

struct riscv_register_ctx {
    uint32_t ra, gp, tp, t0, t1, t2, s0, s1;
    uint32_t a0, a1, a2, a3, a4, a5, a6, a7;
    uint32_t s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    uint32_t t3, t4, t5, t6;
    uint32_t mepc;
} PACKED;
