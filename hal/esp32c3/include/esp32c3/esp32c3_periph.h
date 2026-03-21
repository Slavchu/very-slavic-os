#pragma once
#include <stdint.h>
#include <bitops.h>

/* Defined in esp32c3.peripheral.ld */
/* Sincerely, very-slavic-os developer */
extern volatile uint32_t UART0[];
extern volatile uint32_t UART1[];
extern volatile uint32_t SPIMEM1[];
extern volatile uint32_t SPIMEM0[];
extern volatile uint32_t GPIO[];
extern volatile uint32_t SDM[];
extern volatile uint32_t RTCCNTL[];
extern volatile uint32_t RTCIO[];
extern volatile uint32_t EFUSE[];
extern volatile uint32_t HINF[];
extern volatile uint32_t I2S0[];
extern volatile uint32_t I2C0[];
extern volatile uint32_t UHCI0[];
extern volatile uint32_t HOST[];
extern volatile uint32_t RMT[];
extern volatile uint32_t RMTMEM[];
extern volatile uint32_t SLC[];
extern volatile uint32_t LEDC[];
extern volatile uint32_t TIMERG0[];
extern volatile uint32_t TIMERG1[];
extern volatile uint32_t SYSTIMER[];
extern volatile uint32_t GPSPI2[];
extern volatile uint32_t SYSCON[];
extern volatile uint32_t TWAI[];
extern volatile uint32_t APB_SARADC[];
extern volatile uint32_t USB_SERIAL_JTAG[];
extern volatile uint32_t GDMA[];
extern volatile uint32_t SYSTEM[];
extern volatile uint32_t INTERRUPT[];

/* SYSTIMER registers */
#define SYSTIMER_CONFIG_REG 0x00
#define SYSTIMER_CLK_EN_BIT BIT(31)
#define SYSTIMER_TIMER_UNIT0_WORK_EN BIT(30)
#define SYSTIMER_TIMER_UNIT1_WORK_EN BIT(29)
#define SYSTIMER_TIMER_UNIT0_CORE0_STALL_EN BIT(28)
#define SYSTIMER_TIMER_UNIT1_CORE0_STALL_EN BIT(26)
#define SYSTIMER_TARGET0_WORK_EN_BIT BIT(24)
#define SYSTIMER_TARGET1_WORK_EN_BIT BIT(23)
#define SYSTIMER_TARGET2_WORK_EN_BIT BIT(22)

#define SYSTIMER_INTERRUPT_ENA_REG 0x64
#define SYSTIMER_INTERRUPT_RAW_REG 0x68
#define SYSTIMER_INTERRUPT_CLR_REG 0x6c

#define SYSTIMER_TARGET0_HI_REG 0x1C
#define SYSTIMER_TARGET_HI_BITMASK 0x7FFFF // fisrt 19 bits only, the rest are reserved

#define SYSTIMER_TARGET0_LO_REG 0x0020

#define SYSTIMER_TARGET0_CONF_REG 0x34
#define SYSTIMER_TARGET_TIMER_UNIT_SEL_BIT BIT(31)
#define SYSTIMER_TARGET_PERIOD_MODE_BIT BIT(30)
#define SYSTIMER_TARGET_PERIOD_MASK 0x1FFFFFF

#define SYSTIMER_COMP0_LOAD_REG 0x0050
#define SYSTIMER_COMP_LOAD_REG_MASK 0x01

#define SYSTIMER_TARGET1_HI_REG 0x24
// #define SYSTIMER_TARGET_HI_BITMASK 0x7FFFF  //fisrt 19 bits only, the rest are reserved

#define SYSTIMER_TARGET1_LO_REG 0x0028

#define SYSTIMER_TARGET1_CONF_REG 0x38
// #define SYSTIMER_TARGET_TIMER_UNIT_SEL_BIT BIT(31)
// #define SYSTIMER_TARGET_PERIOD_MODE_BIT BIT(30)
// #define SYSTIMER_TARGET_PERIOD_MASK 0x1FFFFFF

#define SYSTIMER_COMP1_LOAD_REG 0x54
// #define SYSTIMER_COMP_LOAD_REG_MASK 0x01

#define SYSTIMER_TARGET2_HI_REG 0x2C
#define SYSTIMER_TARGET2_LO_REG 0x30

#define SYSTIMER_TARGET2_CONF_REG 0x3C

#define SYSTIMER_COMP2_LOAD_REG 0x58
// #define SYSTIMER_COMP_LOAD_REG_MASK 0x01
/* End of systimer register */

/* INTERRUPT registers */
#define INTERRUPT_CORE0_PWR_INTR_MAP_REG                    0x0008
#define INTERRUPT_CORE0_I2C_MST_INT_MAP_REG                 0x002C
#define INTERRUPT_CORE0_SLC0_INTR_MAP_REG                   0x0030
#define INTERRUPT_CORE0_SLC1_INTR_MAP_REG                   0x0034
#define INTERRUPT_CORE0_APB_CTRL_INTR_MAP_REG               0x0038
#define INTERRUPT_CORE0_UHCI0_INTR_MAP_REG                  0x003C
#define INTERRUPT_CORE0_GPIO_INTERRUPT_PRO_MAP_REG          0x0040
#define INTERRUPT_CORE0_SPI_INTR_1_MAP_REG                  0x0048
#define INTERRUPT_CORE0_SPI_INTR_2_MAP_REG                  0x004C
#define INTERRUPT_CORE0_I2S_INT_MAP_REG                     0x0050
#define INTERRUPT_CORE0_UART_INTR_MAP_REG                   0x0054
#define INTERRUPT_CORE0_UART1_INTR_MAP_REG                  0x0058
#define INTERRUPT_CORE0_LEDC_INT_MAP_REG                    0x005C
#define INTERRUPT_CORE0_EFUSE_INT_MAP_REG                   0x0060
#define INTERRUPT_CORE0_TWAI_INT_MAP_REG                    0x0064
#define INTERRUPT_CORE0_USB_INTR_MAP_REG                    0x0068
#define INTERRUPT_CORE0_RTC_CORE_INTR_MAP_REG               0x006C
#define INTERRUPT_CORE0_RMT_INTR_MAP_REG                    0x0070
#define INTERRUPT_CORE0_I2C_EXT0_INTR_MAP_REG               0x0074
#define INTERRUPT_CORE0_TIMER_INT1_MAP_REG                  0x0078
#define INTERRUPT_CORE0_TIMER_INT2_MAP_REG                  0x007C
#define INTERRUPT_CORE0_TG_T0_INT_MAP_REG                   0x0080
#define INTERRUPT_CORE0_TG_WDT_INT_MAP_REG                  0x0084
#define INTERRUPT_CORE0_TG1_T0_INT_MAP_REG                  0x0088
#define INTERRUPT_CORE0_TG1_WDT_INT_MAP_REG                 0x008C
#define INTERRUPT_CORE0_CACHE_IA_INT_MAP_REG                0x0090
#define INTERRUPT_CORE0_SYSTIMER_TARGET0_INT_MAP_REG        0x0094
#define INTERRUPT_CORE0_SYSTIMER_TARGET1_INT_MAP_REG        0x0098
#define INTERRUPT_CORE0_SYSTIMER_TARGET2_INT_MAP_REG        0x009C
#define INTERRUPT_CORE0_SPI_MEM_REJECT_INTR_MAP_REG         0x00A0
#define INTERRUPT_CORE0_ICACHE_PRELOAD_INT_MAP_REG          0x00A4
#define INTERRUPT_CORE0_ICACHE_SYNC_INT_MAP_REG             0x00A8
#define INTERRUPT_CORE0_APB_ADC_INT_MAP_REG                 0x00AC
#define INTERRUPT_CORE0_DMA_CH0_INT_MAP_REG                 0x00B0
#define INTERRUPT_CORE0_DMA_CH1_INT_MAP_REG                 0x00B4
#define INTERRUPT_CORE0_DMA_CH2_INT_MAP_REG                 0x00B8
#define INTERRUPT_CORE0_RSA_INT_MAP_REG                     0x00BC
#define INTERRUPT_CORE0_AES_INT_MAP_REG                     0x00C0
#define INTERRUPT_CORE0_SHA_INT_MAP_REG                     0x00C4
#define INTERRUPT_CORE0_CPU_INTR_FROM_CPU_0_MAP_REG         0x00C8
#define INTERRUPT_CORE0_CPU_INTR_FROM_CPU_1_MAP_REG         0x00CC
#define INTERRUPT_CORE0_CPU_INTR_FROM_CPU_2_MAP_REG         0x00D0
#define INTERRUPT_CORE0_CPU_INTR_FROM_CPU_3_MAP_REG         0x00D4
#define INTERRUPT_CORE0_ASSIST_DEBUG_INTR_MAP_REG           0x00D8
#define INTERRUPT_CORE0_DMA_APBPERI_PMS_MONITOR_VIOLATE_INTR_MAP_REG 0x00DC
#define INTERRUPT_CORE0_CORE_0_IRAM0_PMS_MONITOR_VIOLATE_INTR_MAP_REG 0x00E0
#define INTERRUPT_CORE0_CORE_0_DRAM0_PMS_MONITOR_VIOLATE_INTR_MAP_REG 0x00E4
#define INTERRUPT_CORE0_CORE_0_PIF_PMS_MONITOR_VIOLATE_INTR_MAP_REG   0x00E8
#define INTERRUPT_CORE0_CORE_0_PIF_PMS_MONITOR_VIOLATE_SIZE_INTR_MAP_REG 0x00EC
#define INTERRUPT_CORE0_BACKUP_PMS_VIOLATE_INTR_MAP_REG     0x00F0
#define INTERRUPT_CORE0_CACHE_CORE0_ACS_INT_MAP_REG         0x00F4

/* Interrupt Source Status Registers */
#define INTERRUPT_CORE0_INTR_STATUS_0_REG                   0x00F8
#define INTERRUPT_CORE0_INTR_STATUS_1_REG                   0x00FC

/* Clock and Control Registers */
#define INTERRUPT_CORE0_CLOCK_GATE_REG                      0x0100
#define INTERRUPT_CORE0_CPU_INT_ENABLE_REG                  0x0104
#define INTERRUPT_CORE0_CPU_INT_TYPE_REG                    0x0108
#define INTERRUPT_CORE0_CPU_INT_CLEAR_REG                   0x010C
#define INTERRUPT_CORE0_CPU_INT_EIP_STATUS_REG              0x0110
#define INTERRUPT_CORE0_CPU_INT_THRESH_REG                  0x0194

/* Priority Configuration Registers */
#define INTERRUPT_CORE0_CPU_INT_PRI_1_REG                   0x0118
#define INTERRUPT_CORE0_CPU_INT_PRI_2_REG                   0x011C
#define INTERRUPT_CORE0_CPU_INT_PRI_3_REG                   0x0120
#define INTERRUPT_CORE0_CPU_INT_PRI_4_REG                   0x0124
#define INTERRUPT_CORE0_CPU_INT_PRI_5_REG                   0x0128
#define INTERRUPT_CORE0_CPU_INT_PRI_6_REG                   0x012C
#define INTERRUPT_CORE0_CPU_INT_PRI_7_REG                   0x0130
#define INTERRUPT_CORE0_CPU_INT_PRI_8_REG                   0x0134
#define INTERRUPT_CORE0_CPU_INT_PRI_9_REG                   0x0138
#define INTERRUPT_CORE0_CPU_INT_PRI_10_REG                  0x013C
#define INTERRUPT_CORE0_CPU_INT_PRI_11_REG                  0x0140
#define INTERRUPT_CORE0_CPU_INT_PRI_12_REG                  0x0144
#define INTERRUPT_CORE0_CPU_INT_PRI_13_REG                  0x0148
#define INTERRUPT_CORE0_CPU_INT_PRI_14_REG                  0x014C
#define INTERRUPT_CORE0_CPU_INT_PRI_15_REG                  0x0150
#define INTERRUPT_CORE0_CPU_INT_PRI_16_REG                  0x0154
#define INTERRUPT_CORE0_CPU_INT_PRI_17_REG                  0x0158
#define INTERRUPT_CORE0_CPU_INT_PRI_18_REG                  0x015C
#define INTERRUPT_CORE0_CPU_INT_PRI_19_REG                  0x0160
#define INTERRUPT_CORE0_CPU_INT_PRI_20_REG                  0x0164
#define INTERRUPT_CORE0_CPU_INT_PRI_21_REG                  0x0168
#define INTERRUPT_CORE0_CPU_INT_PRI_22_REG                  0x016C
#define INTERRUPT_CORE0_CPU_INT_PRI_23_REG                  0x0170
#define INTERRUPT_CORE0_CPU_INT_PRI_24_REG                  0x0174
#define INTERRUPT_CORE0_CPU_INT_PRI_25_REG                  0x0178
#define INTERRUPT_CORE0_CPU_INT_PRI_26_REG                  0x017C
#define INTERRUPT_CORE0_CPU_INT_PRI_27_REG                  0x0180
#define INTERRUPT_CORE0_CPU_INT_PRI_28_REG                  0x0184
#define INTERRUPT_CORE0_CPU_INT_PRI_29_REG                  0x0188
#define INTERRUPT_CORE0_CPU_INT_PRI_30_REG                  0x018C
#define INTERRUPT_CORE0_CPU_INT_PRI_31_REG                  0x0190

/* System registers*/
#define SYSTEM_PERIP_CLK_EN0_REG 0x10
#define SYSTEM_SYSTIMER_CLK_EN_BIT BIT(29)

#define SYSTEM_PERIP_RST_EN0_REG 0x18
#define SYSTEM_SYSTIMER_RST_BIT 
