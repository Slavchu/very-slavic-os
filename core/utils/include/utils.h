#pragma once

#define UNUSED __attribute__((unused))
#define USED __attribute__((used))
#define PACKED __attribute__((packed))

#define TIME_MS_IN_S 1000
#define TIME_MS_TO_S(x) ((x)/TIME_MS_IN_S)
#define TIME_S_TO_MS(x) ((x)*TIME_MS_IN_S)

#define _XSTR_IMPL(x) #x
// stringify a whatever you put here
#define XSTR(x) _XSTR_IMPL(x)

#define C_RESET   "\033[0m"
#define C_RED     "\033[31m"
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_BLUE    "\033[34m"
#define C_MAGENTA "\033[35m"
#define C_CYAN    "\033[36m"
#define C_WHITE   "\033[37m"
#define C_B_RED   "\033[1;31m"
#define C_B_GREEN "\033[1;32m"
#define C_B_YELLOW "\033[1;33m"
#define C_B_WHITE "\033[1;37m"

#define _COLORIFY_IMPL (color, str) color str C_RESET
#define COLORIFY(color, str) _COLORIFY_IMPL(color, str)
