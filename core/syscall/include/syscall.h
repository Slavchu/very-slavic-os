#pragma once

#include <hal/context_operations.h>
#include <stdint.h>

enum syscall_reason : unsigned {
    SYSCALL_REASON_YIELD = 0,
    SYSCALL_MAX,
};

typedef uint32_t (*syscall_cb_t)();

void syscall_bind(enum syscall_reason reason, syscall_cb_t cb);

uint32_t syscall_invoke(enum syscall_reason reason);

uint32_t syscall_proceed(unsigned reason);
