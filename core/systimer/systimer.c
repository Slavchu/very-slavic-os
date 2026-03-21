#include <systimer.h>
#include <ets_sys.h>

int flag = 0;

void systimer_tick() {
    flag = 1;
}
