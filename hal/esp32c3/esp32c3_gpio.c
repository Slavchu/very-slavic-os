#include <esp32c3/esp32c3_periph.h>
#include <hal/gpio.h>

void gpio_set_mode(uint16_t gpio_num, enum gpio_mode mode) {
    if (gpio_num >= 22)
        return;

    // Отримуємо покажчик на регістр IO_MUX для конкретного піна
    volatile uint32_t *io_mux_reg = REG_IO_MUX_GPIO(gpio_num);
    uint32_t io_mux_val = *io_mux_reg;

    // 1. Очищаємо старі налаштування (функцію та підтяжки)
    io_mux_val &= ~MCU_SEL_M;
    io_mux_val &= ~(FUN_IE | FUN_WPU | FUN_WPD);

    // 2. Встановлюємо функцію FUNC1 (GPIO) для ESP32-C3
    // (зсув на 12 біт, згідно з вашим макросом MCU_SEL_M)
    io_mux_val |= (1 << 12);

    // 3. Застосовуємо налаштування залежно від режиму
    switch (mode) {
    case GPIO_MODE_INPUT:
        io_mux_val |= FUN_IE;
        *io_mux_reg = io_mux_val;
        GPIO.enable_w1tc = (1 << gpio_num);
        break;

    case GPIO_MODE_INPUT_PULLUP:
        io_mux_val |= (FUN_IE | FUN_WPU);
        *io_mux_reg = io_mux_val;
        GPIO.enable_w1tc = (1 << gpio_num);
        break;

    case GPIO_MODE_INPUT_PULLDOWN:
        io_mux_val |= (FUN_IE | FUN_WPD);
        *io_mux_reg = io_mux_val;
        GPIO.enable_w1tc = (1 << gpio_num);
        break;

    case GPIO_MODE_OUTPUT:
        io_mux_val |= FUN_IE;
        *io_mux_reg = io_mux_val;

        GPIO.enable_w1ts = (1 << gpio_num);

        volatile uint32_t *func_out_reg = REG_GPIO_FUNC_OUT_SEL_CFG(gpio_num);
        *func_out_reg = 128;
        break;
    }
}

void gpio_set(uint16_t gpio_num, bool state) {
    if (gpio_num >= 22)
        return;

    if (state) {
        // Атомарне встановлення (set)
        GPIO.out_w1ts = (1UL << gpio_num);
    } else {
        // Атомарне скидання (reset)
        GPIO.out_w1tc = (1UL << gpio_num);
    }
}

bool gpio_get(uint16_t gpio_num) {
    if (gpio_num >= 22)
        return false;

    return (GPIO.in & (1 << gpio_num)) != 0;
}
