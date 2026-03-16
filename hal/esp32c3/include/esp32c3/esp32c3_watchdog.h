#pragma once
#include <bitops.h>

/* Keys */
#define ESP32C3_WDT_WPROTECT_KEY 0x50D83AA1

/* Offsets for Timer Groups */
#define ESP32C3_TIMG_WDTCONFIG0_REG 0x48
#define ESP32C3_TIMG_WDTWPROTECT_REG 0x64

/* Offsets for RTC */
#define ESP32C3_RTC_WDTCONFIG0_REG 0x98
#define ESP32C3_RTC_WDTWPROTECT_REG 0xA4

/* Bit masks */
#define ESP32C3_TIMG_WDT_FLASHBOOT_MOD_EN   BIT(14)
#define ESP32C3_RTC_WDT_FLASHBOOT_MOD_EN    BIT(10)
#define ESP32C3_WDT_EN_BIT BIT(31)
