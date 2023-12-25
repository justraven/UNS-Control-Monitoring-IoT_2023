#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include "server.h"
#include "request.h"

#define CONNECTIONS_AP_SSID "UNS-DEV-"
#define CONNECTIONS_AP_PASS "12345678"

typedef enum {
    CONNECTIONS_SUCCESS,
    CONNECTIONS_FAIL,
    CONNECTIONS_SPIFFS_ERR_CONFIG, // SPIFFS cannot read connections.data
    CONNECTIONS_SPIFFS_ERR, // there is no SPIFFS memory allocated
    CONNECTIONS_SPIFFS_OK
} connections_flags_t;

typedef struct {
    String ssid;
    String password;
    String ipaddr;
    String macaddr;
} connections_hwdata_t;

connections_flags_t connections_init (void);

connections_flags_t connections_read_config (connections_hwdata_t *hwdata);
connections_flags_t connections_start_ap (connections_hwdata_t *hwdata);

#endif // CONNECTIONS_H