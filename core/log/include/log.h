#pragma once
#include <stdint.h>
#include <utils.h>

#ifndef ME
#define ME ""
#endif

typedef enum : uint8_t {
    LEVEL_CRITICAL = 0,
    LEVEL_ERROR,
    LEVEL_INFO,
    LEVEL_DEBUG,
} log_level_t;

#ifndef MODULE_LOG_LEVEL
#define MODULE_LOG_LEVEL LOG_LEVEL_DEFAULT
#endif

#ifndef LOG_PRINTF_FUNC
#define LOG_PRINTF_FUNC _print_log
#endif // LOG_PRINTF_FUNC

//do not use. USE LOG_##LEVEL instead. For example :LOG_INFO("Some log %s", some_str)
#define _LOG_IMPL(level, fmt, ...) LOG_PRINTF_FUNC("%s " ME ":\t" fmt, level_to_string(level), ##__VA_ARGS__)

#define LOG_CRITICAL(fmt, ...) _LOG_IMPL(LEVEL_CRITICAL, fmt, ##__VA_ARGS__)

#if MODULE_LOG_LEVEL > 0
#define LOG_ERROR(fmt, ...) _LOG_IMPL(LEVEL_ERROR, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...)
#endif

#if MODULE_LOG_LEVEL > 1
#define LOG_INFO(fmt, ...) _LOG_IMPL(LEVEL_INFO, fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...)
#endif

#if MODULE_LOG_LEVEL > 2
#define LOG_DEBUG(fmt, ...) _LOG_IMPL(LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...)
#endif

void _print_log(const char *fmt, ...);

const char *level_to_string(log_level_t level);
