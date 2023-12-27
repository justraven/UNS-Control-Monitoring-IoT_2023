#ifndef DEVICE_TASK_H
#define DEVICE_TASK_H

#include <Arduino.h>
#include "flags.h"
#include "status.h"
#include "sensors.h"
#include "actuators.h"
#include "systime.h"
#include "connections.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

void device_task_init (void);

/* run forever task */
void dvtask_time_keeping (void *pvParameters);
#ifndef DEVICE_CONTROLLER_OUTSIDE
void dvtask_sensors (void *pvParameters);
#endif // DEVICE_CONTROLLER_OUTSIDE
void dvtask_handle_client (void *pvParameters);

#ifdef DEVICE_CONTROLLER_OUTSIDE
void dvtask_monitor_actuators (void *pvParameters);
#endif // DEVICE_CONTROLLER_OUTSIDE

/* run once task */
void dvtask_single_establish_connection (void);

#endif // DEVICE_TASK_H