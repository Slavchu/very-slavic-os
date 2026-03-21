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
