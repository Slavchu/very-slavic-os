#include <hal/hal.h>
#include <log.h>
#include <syscall.h>

static syscall_cb_t syscall_table[SYSCALL_MAX] = {0};

void syscall_bind(enum syscall_reason reason, syscall_cb_t cb) {
    syscall_table[(unsigned)reason] = cb;
}

uint32_t syscall_invoke(enum syscall_reason reason) {
    return hal_syscall_invoke((unsigned)reason);
}

uint32_t syscall_proceed(unsigned reason) {
    return syscall_table[(unsigned)reason]();
}
