#ifndef DEVICE_TASK_H
#define DEVICE_TASK_H

#include <Arduino.h>
#include "flags.h"
#include "status.h"
#include "sensors.h"
#include "actuators.h"
#include "connections.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} time_keeping_t;

void device_task_init (void);

/* run forever task */
void dvtask_time_keeping (void *pvParameters);
void dvtask_sensors (void *pvParameters);
void dvtask_handle_client (void *pvParameters);

/* run once task */
void dvtask_single_establish_connection (void);

/* API */
uint8_t time_keeping_compare (time_keeping_t first, time_keeping_t second);
uint8_t time_keeping_multiple_mins (time_keeping_t first, time_keeping_t multiple);

#endif // DEVICE_TASK_H