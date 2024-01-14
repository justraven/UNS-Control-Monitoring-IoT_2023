#ifndef SYSTIME_H
#define SYSTIME_H

#include <Arduino.h>

typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} time_keeping_t;

time_keeping_t time_keeping_get (void);
time_keeping_t time_keeping_set (time_keeping_t time);

/* API */
uint8_t time_keeping_compare (time_keeping_t first, time_keeping_t second);
uint8_t time_keeping_multiple_mins (time_keeping_t first, time_keeping_t multiple);
uint8_t time_keeping_multiple_hours (time_keeping_t first, time_keeping_t multiple);

#endif // SYSTIME_H


