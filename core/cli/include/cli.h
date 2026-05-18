#pragma once

#define CLI_MAX_COMMANDS 20
#define CLI_MAX_ARGS 10
#define CLI_BUFFER_SIZE 128

typedef void (*cli_cmd_cb_t)(int argc, char **argv);

int cli_register_command(const char *name, const char *description, cli_cmd_cb_t callback);
void cli_printf(const char *format, ...);
void cli_block_output();
void cli_unblock_output();
void cli_block_input();
void cli_unblock_input();
