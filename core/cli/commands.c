#include <cli.h>
#include <func_table.h>
#include <hal/gpio.h>
#include <hal/systimer.h>
#include <scheduler.h>
#include <stdlib.h>
#include <utils.h>

#if MODULE_SNAKE
#include <snake.h>
#endif

USED static void cmd_uptime(int argc UNUSED, char **argv UNUSED) {
    cli_printf("Uptime: %u seconds\r\n", TIME_MS_TO_S((uint32_t)(hal_systimer_get_sys_time_ms())));
}

USED static void cmd_gpio(int argc, char **argv) {
    if (argc < 3) {
        goto err;
    }

    unsigned current_arg = 1;
    bool set_mode = false;

    if (*argv[current_arg] == 's') {
        if (argc < 4) {
            goto err;
        }
        current_arg++;
        set_mode = true;
    } else if (*argv[current_arg] == 'g') {
        current_arg++;
        set_mode = false;
    } else {
        goto err;
    }

    int gpio_num = atoi(argv[current_arg]);
    current_arg++;

    if (set_mode) {
        int state = atoi(argv[current_arg]);
        if (state != 0 && state != 1) {
            goto err;
        }

        gpio_set_mode((uint16_t)gpio_num, GPIO_MODE_OUTPUT);
        gpio_set((uint16_t)gpio_num, (bool)state);
        cli_printf(C_GREEN "OK: GPIO %d set to %d\r\n" C_RESET, gpio_num, state);
    } else {
        gpio_set_mode((uint16_t)gpio_num, GPIO_MODE_INPUT);
        bool state = gpio_get((uint16_t)gpio_num);
        cli_printf("GPIO %d value: " C_GREEN "%d\r\n" C_RESET, gpio_num, state);
    }
    return;
err:
    cli_printf("Syntax error: Use gpio s [pin] [state(1/0)] or gpio g [pin]\r\n");
}

static uint32_t blink_period = 0;
static uint16_t blink_gpio_num = 0;

USED void blink_task() {
    bool state = false;
    while (1) {
        state = !state;
        gpio_set((uint16_t)blink_gpio_num, state);
        sleep(blink_period);
    }
}

USED static void cmd_blink(int argc, char **argv) {
    if (argc < 3) {
        return;
    }

    blink_gpio_num = atoi(argv[1]);
    blink_period = atoi(argv[2]);

    if (blink_gpio_num < 0 || blink_gpio_num >= 22) {
        goto err;
        return;
    }

    if (blink_period <= 0) {
        goto err;
        return;
    }

    gpio_set_mode((uint16_t)blink_gpio_num, GPIO_MODE_OUTPUT);
    scheduler_create_task(blink_task, TASK_PRIORITY_LOW);
    return;
err:
    cli_printf(C_RED "ERROR: Usage: blink [gpio_num] [period_ms]\r\n" C_RESET);
    return;
}

USED static void commands_init(void *UNUSED) {
    cli_register_command("uptime", "Show system uptime", cmd_uptime);
    cli_register_command("gpio", "Gpio operations", cmd_gpio);
    cli_register_command("blink", "Gpio blink", cmd_blink);
#ifdef MODULE_SNAKE
    cli_register_command("snake", "Snake the game", cmd_snake);
#endif
}
FUNC_TABLE_APPEND(FUNC_TABLE_INIT, commands_init);
