#include <cli.h>
#include <func_table.h>
#include <mutex.h>
#include <scheduler.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>

typedef struct {
    const char *name;
    const char *description;
    cli_cmd_cb_t callback;
} cli_command_t;

static cli_command_t cmd_table[CLI_MAX_COMMANDS];
static int cmd_count = 0;
static char rx_buffer[CLI_BUFFER_SIZE];
static int rx_index = 0;
static const char *prompt = "CLI> ";
static struct mutex cli_output_mtx;
static struct mutex cli_input_mtx;

static void cli_redraw_prompt(void) {
    printf("\r%s%s", prompt, rx_buffer);
    fflush(stdout);
}

static void cli_execute(void) {
    printf("\r\n");

    if (rx_index == 0) {
        cli_redraw_prompt();
        return;
    }

    char *argv[CLI_MAX_ARGS];
    int argc = 0;
    char tmp_buf[CLI_BUFFER_SIZE];
    memcpy(tmp_buf, rx_buffer, CLI_BUFFER_SIZE);
    rx_index = 0;
    memset(rx_buffer, 0, CLI_BUFFER_SIZE);

    char *token = strtok(tmp_buf, " ");
    while (token != NULL && argc < CLI_MAX_ARGS) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    if (argc > 0) {
        int found = 0;
        for (int i = 0; i < cmd_count; i++) {
            if (strcmp(cmd_table[i].name, argv[0]) == 0) {
                cmd_table[i].callback(argc, argv);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Unknown command: %s\r\n", argv[0]);
        }
    }

    cli_redraw_prompt();
}

int cli_register_command(const char *name, const char *description, cli_cmd_cb_t callback) {
    int res = -1;
    mutex_lock(&cli_output_mtx);
    if (cmd_count < CLI_MAX_COMMANDS) {
        cmd_table[cmd_count].name = name;
        cmd_table[cmd_count].description = description;
        cmd_table[cmd_count].callback = callback;
        cmd_count++;
        res = 0;
    }
    mutex_unlock(&cli_output_mtx);
    return res;
}

static void cli_process_escape() {
    getchar();
    getchar();
}

void cli_process_char(char c) {
    mutex_lock(&cli_output_mtx);
    switch (c) {
    case '\r':
    case '\n':
        rx_buffer[rx_index] = '\0';
        cli_execute();
        break;
    case '\b':
        if (rx_index > 0) {
            rx_index--;
            rx_buffer[rx_index] = '\0';
            printf("\b \b");
            fflush(stdout);
        }
        break;
    case '\033':
        cli_process_escape();
        break;
    default:
        if (rx_index < CLI_BUFFER_SIZE - 1) {
            rx_buffer[rx_index++] = c;
            putchar(c);
            fflush(stdout);
        }
        break;
    }
    mutex_unlock(&cli_output_mtx);
}

void cli_printf(const char *format, ...) {
    mutex_lock(&cli_output_mtx);
    printf("\r\033[K");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    cli_redraw_prompt();
    mutex_unlock(&cli_output_mtx);
}

static void cli_task(void) {
    while (1) {
        mutex_lock(&cli_input_mtx);
        char c = getchar();
        cli_process_char(c);
        mutex_unlock(&cli_input_mtx);
    }
}

static void cli_command_help(int argc UNUSED, char *argv[] UNUSED) {
    for (int i = 0; i < cmd_count; i++) {
        printf("\t%s\t%s\r\n", cmd_table[i].name, cmd_table[i].description);
    }
}

void cli_block_output() {
    mutex_lock(&cli_output_mtx);
}

void cli_unblock_output() {
    mutex_unlock(&cli_output_mtx);
}

void cli_block_input() {
    mutex_lock(&cli_input_mtx);
}

void cli_unblock_input() {
    mutex_unlock(&cli_input_mtx);
}

static void cli_init(void *UNUSED) {
    mutex_init(&cli_output_mtx);
    mutex_init(&cli_input_mtx);
    cmd_count = 0;
    rx_index = 0;
    memset(rx_buffer, 0, CLI_BUFFER_SIZE);
    printf("\r\n");
    cli_redraw_prompt();
    scheduler_create_task(cli_task, TASK_PRIORITY_LOW);
    cli_register_command("help", "Show this help", cli_command_help);
}
FUNC_TABLE_APPEND(FUNC_TABLE_INIT, cli_init)
