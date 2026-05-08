#pragma once
#include <stdint.h>

struct mutex {
    volatile uint8_t is_locked;
    volatile uint32_t locker;
    volatile uint16_t blocker_id;
};

void mutex_init(struct mutex *mutex);

void mutex_deinit(struct mutex *mutex);

void mutex_lock(struct mutex *mutex);

void mutex_unlock(struct mutex *mutex);
