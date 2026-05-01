#pragma once
#include <stdint.h>

struct esp32c3_interrupt_matrix {
    volatile uint32_t reserved_0x00[2];
    volatile uint32_t pwr_intr_map;
    volatile uint32_t reserved_0x0C[8];

    volatile uint32_t i2c_mst_int_map;
    volatile uint32_t slc0_intr_map;
    volatile uint32_t slc1_intr_map;
    volatile uint32_t apb_ctrl_intr_map;
    volatile uint32_t uhci0_intr_map;
    volatile uint32_t gpio_interrupt_pro_map;

    volatile uint32_t reserved_0x44;

    volatile uint32_t spi_intr_1_map;
    volatile uint32_t spi_intr_2_map;
    volatile uint32_t i2s_int_map;
    volatile uint32_t uart_intr_map;
    volatile uint32_t uart1_intr_map;
    volatile uint32_t ledc_int_map;
    volatile uint32_t efuse_int_map;
    volatile uint32_t twai_int_map;
    volatile uint32_t usb_intr_map;
    volatile uint32_t rtc_core_intr_map;
    volatile uint32_t rmt_intr_map;
    volatile uint32_t i2c_ext0_intr_map;
    volatile uint32_t timer_int1_map;
    volatile uint32_t timer_int2_map;
    volatile uint32_t tg_t0_int_map;
    volatile uint32_t tg_wdt_int_map;
    volatile uint32_t tg1_t0_int_map;
    volatile uint32_t tg1_wdt_int_map;
    volatile uint32_t cache_ia_int_map;
    volatile uint32_t systimer_target0_map;
    volatile uint32_t systimer_target1_map;
    volatile uint32_t systimer_target2_map;
    volatile uint32_t spi_mem_reject_intr_map;
    volatile uint32_t icache_preload_int_map;
    volatile uint32_t icache_sync_int_map;
    volatile uint32_t apb_adc_int_map;
    volatile uint32_t dma_ch0_int_map;
    volatile uint32_t dma_ch1_int_map;
    volatile uint32_t dma_ch2_int_map;
    volatile uint32_t rsa_int_map;
    volatile uint32_t aes_int_map;
    volatile uint32_t sha_int_map;
    volatile uint32_t cpu_intr_from_cpu_0_map;
    volatile uint32_t cpu_intr_from_cpu_1_map;
    volatile uint32_t cpu_intr_from_cpu_2_map;
    volatile uint32_t cpu_intr_from_cpu_3_map;
    volatile uint32_t assist_debug_intr_map;
    volatile uint32_t dma_apbperi_pms_monitor_violate_intr_map;
    volatile uint32_t core_0_iram0_pms_monitor_violate_intr_map;
    volatile uint32_t core_0_dram0_pms_monitor_violate_intr_map;
    volatile uint32_t core_0_pif_pms_monitor_violate_intr_map;
    volatile uint32_t core_0_pif_pms_monitor_violate_size_intr_map;
    volatile uint32_t backup_pms_violate_intr_map;
    volatile uint32_t cache_core0_acs_int_map;

    volatile uint32_t intr_status_0;
    volatile uint32_t intr_status_1;

    volatile uint32_t clock_gate;
    volatile uint32_t int_enable;
    volatile uint32_t int_type;
    volatile uint32_t int_clear;
    volatile uint32_t int_eip_status;

    volatile uint32_t pri[32];
    volatile uint32_t int_thresh;
};
