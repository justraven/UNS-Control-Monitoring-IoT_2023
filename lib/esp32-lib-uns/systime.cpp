/*
 * systime.cpp
 * Author : Muhammad Shofuwan Anwar
 * Created : 2023-12-20 00:21:00
 */

#include "systime.h"

static time_keeping_t __time_keeping = {0};

time_keeping_t time_keeping_get (void) {
    return __time_keeping;
}
time_keeping_t time_keeping_set (time_keeping_t time) {
    __time_keeping = time;
    return __time_keeping;
}

uint8_t time_keeping_compare (time_keeping_t first, time_keeping_t second) {
    if (first.hour == second.hour && first.minute == second.minute && first.second == second.second) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t time_keeping_multiple_mins (time_keeping_t first, time_keeping_t multiple) {
    if ((first.minute % multiple.minute) == 0) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t time_keeping_multiple_hours (time_keeping_t first, time_keeping_t multiple) {
    if ((first.hour % multiple.hour) == 0) {
        return 1;
    } else {
        return 0;
    }
}


