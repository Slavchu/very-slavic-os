#include <log.h>
#include <stdarg.h>

extern int ets_vprintf(const char *fmt, va_list ap);

const char *level_to_string(log_level_t level) {
    switch (level) {
    case LEVEL_CRITICAL:
        return "ERROR_CRITICAL";
    case LEVEL_ERROR:
        return "LEVEL_ERROR";
    case LEVEL_INFO:
        return "LEVEL_INFO";
    case LEVEL_DEBUG:
        return "LEVEL_DEBUG";
    default:
        return "UNKNOWN_LEVEL";
    }
}

void _print_log(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    // some vprintf function
    va_end(args);
}
