#pragma once
#include <stdint.h>

enum gpio_mode {
    GPIO_MODE_OUTPUT,
    GPIO_MODE_INPUT,
    GPIO_MODE_INPUT_PULLUP,
    GPIO_MODE_INPUT_PULLDOWN,
};

void gpio_set_mode(uint16_t gpio_num, enum gpio_mode mode);
void gpio_set(uint16_t gpio_num, bool state);
bool gpio_get(uint16_t gpio_num);
