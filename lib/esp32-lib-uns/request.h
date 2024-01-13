#ifndef REQUEST_H
#define REQUEST_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_efuse.h>
#include "flags.h"

#if defined(DEVICE_CONTROLLER_INSIDE0) || defined(DEVICE_CONTROLLER_INSIDE1) || defined(DEVICE_SOIL_MONITOR0) || defined(DEVICE_CONTROLLER_OUTSIDE)
#define FIELD_TEMPERATURES0     1
#define FIELD_TEMPERATURES1     4
#define FIELD_SOIL_MOISTURE0    8

#define THINGSPEAK_API_KEY  "4ALZ0ANO61B85JSE"
#define THINGSPEAK_CH_ID    2388847 // channel id device-field0
#elif defined(DEVICE_SOIL_MONITOR1) || defined(DEVICE_SOIL_MONITOR2) || defined(DEVICE_SOIL_MONITOR3) || defined(DEVICE_SOIL_MONITOR4) || defined(DEVICE_MONITOR_OLD)
#define THINGSPEAK_API_KEY  "NUJV66HPZH1GQ5B0"
#define THINGSPEAK_CH_ID    2388851 // channel id device-field1
#elif defined(DEVICE_SOIL_MONITOR5) || defined(DEVICE_SOIL_MONITOR6) || defined(DEVICE_SOIL_MONITOR7) || defined(DEVICE_SOIL_MONITOR8)
#define THINGSPEAK_API_KEY  "GW2M5CLQX4VN0LK9"
#define THINGSPEAK_CH_ID    2390292 // channel id device-field2
#endif

typedef enum {
    REQUEST_HTTP_OK = 200,
    REQUEST_HTTP_ERR = 400,
} request_flags_t;

typedef struct {
    const char *api_key;
    float field[8]; // maximum fields in thingspeak
} thingspeak_t;

typedef union {
    uint8_t mac8[6];
    uint32_t mac32[2];
} mac_addr_t;

request_flags_t request_send_get_http (thingspeak_t *thingspeak);
request_flags_t request_read_get_http (float *value, uint8_t field);

#endif // REQUEST_H