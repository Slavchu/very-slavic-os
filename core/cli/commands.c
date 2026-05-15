#include <cli.h>
#include <hal/systimer.h>
#include <utils.h>
#include <func_table.h>

static void cmd_test(int argc UNUSED, char **argv UNUSED) {
    cli_printf("Command executed with %d args\r\n", argc);
}

static void cmd_uptime(int argc UNUSED, char **argv UNUSED) {
    cli_printf("Uptime: %llu seconds\r\n", TIME_MS_TO_S(hal_systimer_get_sys_time_ms()));
}

static void commands_init(void *UNUSED) {
    cli_register_command("test", "Just a test command", cmd_test);
    cli_register_command("uptime", "Show system uptime", cmd_uptime);
}
FUNC_TABLE_APPEND(FUNC_TABLE_INIT, commands_init);
