#ifndef REQUEST_H
#define REQUEST_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_efuse.h>
#include "flags.h"

#ifdef DEVICE_MODE_FULL_FEATURES
#define THINGSPEAK_API_KEY  "4ALZ0ANO61B85JSE"
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

#endif // REQUEST_H