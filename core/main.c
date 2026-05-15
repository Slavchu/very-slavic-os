#include <func_table.h>
#include <hal/hal.h>
#include <hal/interrupt.h>
#include <hal/watchdog.h>
#include <log.h>
#include <mutex.h>
#include <scheduler.h>
#include <utils.h>

const char *greetings_to_traveler = 
  C_B_YELLOW "======================================================\r\n"
               "                  VERY SLAVIC OS\r\n"
               "======================================================\r\n" C_RESET
    "In the beginning, Svarog forged the Firmware from\r\n"
    "bare metal and raw hex.\r\n"
    "\r\n"
    "Now, " C_B_RED "Perun" C_RESET " strikes the MCU with hardware interrupts,\r\n"
    "his hammer ready to smite infinite loops via the Watchdog.\r\n"
    C_CYAN "Stribog" C_RESET " breathes life into the I2C and SPI buses,\r\n"
    "routing bytes across the PCB tracks. " C_MAGENTA "Veles" C_RESET " guards the\r\n"
    "dark, non-volatile pages of Flash memory and EEPROM,\r\n"
    "while " C_BLUE "Mokosh" C_RESET " carefully weaves the threads of the OS\r\n"
    "scheduler, protecting your stack from overflow.\r\n"
    "\r\n"
    C_B_GREEN "[ OK ]" C_RESET " Perun intilized hardware.\r\n"
    C_B_GREEN "[ OK ]" C_RESET " Stribog blessed the traveler.\r\n"
    "\r\n"
    C_B_YELLOW "May the Gods protect your uptime and keep\r\n"
    "Hard Faults at bay! CLI is yours.\r\n"
    "======================================================\r\n" C_RESET;
void os_main() {
    run_table(FUNC_TABLE_INIT, NULL);
    cli_printf("%s\r\n", greetings_to_traveler);
    interrupt_enable_isr();
    while (1) {
        sleep(5000);
    }
}

void main() {
    run_table(FUNC_TABLE_PREINIT, 0);
    scheduler_init(os_main);

    while (1)
        ;
}
