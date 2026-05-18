#include <cli.h>
#include <func_table.h>
#include <hal/gpio.h>
#include <hal/systimer.h>
#include <scheduler.h>
#include <snake.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

static void cmd_test(int argc UNUSED, char **argv UNUSED) {
    cli_printf("Command executed with %d args\r\n", argc);
}

static void cmd_uptime(int argc UNUSED, char **argv UNUSED) {
    cli_printf("Uptime: %llu seconds\r\n", TIME_MS_TO_S(hal_systimer_get_sys_time_ms()));
}

static void cmd_gpio(int argc, char **argv) {
    if (argc < 3) {
        cli_printf(C_RED "ERROR: Not enough arguments\r\n" C_RESET);
        return;
    }

    unsigned current_arg = 1;
    bool set_mode = false;

    if (!strncmp("set", argv[current_arg], 3)) {
        if (argc < 4) {
            cli_printf(C_RED "ERROR: Missing state (1/0) for 'set'\r\n" C_RESET);
            return;
        }
        current_arg++;
        set_mode = true;
    } else if (!strncmp("get", argv[current_arg], 3)) {
        current_arg++;
        set_mode = false;
    } else {
        cli_printf(C_RED "ERROR: Expected 'get' or 'set'\r\n" C_RESET);
        return;
    }

    int gpio_num = atoi(argv[current_arg]);
    if (gpio_num < 0 || gpio_num >= 22) { // Валідація для ESP32-C3
        cli_printf(C_RED "ERROR: Invalid GPIO pin number (0-21)\r\n" C_RESET);
        return;
    }
    current_arg++;

    if (set_mode) {
        int state = atoi(argv[current_arg]);
        if (state != 0 && state != 1) {
            cli_printf(C_RED "ERROR: Invalid state, expected 1 or 0\r\n" C_RESET);
            return;
        }

        gpio_set_mode((uint16_t)gpio_num, GPIO_MODE_OUTPUT);
        gpio_set((uint16_t)gpio_num, (bool)state);
        cli_printf(C_GREEN "OK: GPIO %d set to %d\r\n" C_RESET, gpio_num, state);
    } else {
        gpio_set_mode((uint16_t)gpio_num, GPIO_MODE_INPUT);
        bool state = gpio_get((uint16_t)gpio_num);
        cli_printf("GPIO %d value: " C_GREEN "%d\r\n" C_RESET, gpio_num, state);
    }
}

static uint32_t blink_period = 0;
static uint16_t blink_gpio_num = 0;

void blink_task() {
    bool state = false;
    while (1) {
        state = !state;
        gpio_set((uint16_t)blink_gpio_num, state);
        sleep(blink_period);
    }
}

static void cmd_blink(int argc, char **argv) {
    if (argc < 3) {
        cli_printf(C_RED "ERROR: Not enough arguments. Usage: blink [gpio_num] [period_ms]\r\n" C_RESET);
        return;
    }

    blink_gpio_num = atoi(argv[1]);
    blink_period = atoi(argv[2]);

    if (blink_gpio_num < 0 || blink_gpio_num >= 22) {
        cli_printf(C_RED "ERROR: Invalid GPIO pin number (0-21)\r\n" C_RESET);
        return;
    }

    if (blink_period <= 0) {
        cli_printf(C_RED "ERROR: Period must be greater than 0\r\n" C_RESET);
        return;
    }

    gpio_set_mode((uint16_t)blink_gpio_num, GPIO_MODE_OUTPUT);
    scheduler_create_task(blink_task, TASK_PRIORITY_LOW);

    cli_printf("Blink finished.\r\n");
}

static void commands_init(void *UNUSED) {
    cli_register_command("test", "Just a test command", cmd_test);
    cli_register_command("uptime", "Show system uptime", cmd_uptime);
    cli_register_command("snake", "Snake the game", cmd_snake);
    cli_register_command("gpio", "Gpio operations", cmd_gpio);
    cli_register_command("blink", "Gpio blink", cmd_blink);
}
FUNC_TABLE_APPEND(FUNC_TABLE_INIT, commands_init);
