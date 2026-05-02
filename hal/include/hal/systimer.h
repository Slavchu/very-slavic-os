#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef void (*hal_alarm_cb_t)(void *priv);

typedef int16_t hal_alarm_id_t;

void hal_systimer_init();

/**
 * @brief Set systimer alarm
 *
 * @param delay_ms delay parameter
 * @param cb alarm callback function
 * @param is_periodic if true callback will be called with period delay_ms
 * @param priv private data to pass to callback
 * @retval id of alarm in case of success, or negative value in case of error
 */
hal_alarm_id_t hal_systimer_alarm_set(uint32_t delay_ms, hal_alarm_cb_t cb, bool is_periodic, void *priv);

uint64_t get_sys_time_ms();
