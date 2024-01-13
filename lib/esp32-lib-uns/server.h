#ifndef SERVER_H
#define SERVER_H

#include <Arduino.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include "flags.h"
#include "sensors.h"
#include "systime.h"
#include "status.h"
#include "actuators.h"

typedef enum {
    SERVER_SUCCESS,
    SERVER_FAIL,
    SERVER_SPIFFS_ERR_CONFIG, // SPIFFS cannot read server.data
    SERVER_SPIFFS_ERR, // there is no SPIFFS memory allocated
    SERVER_SPIFFS_OK
} server_flags_t;

typedef struct {
    String ssid;
    String password;
} server_config_t;

void server_init (void);

void server_handle_client (void); // handle client request
void server_handle_home (void); // show home page
void server_handle_configure (void); // handle user form input to change ssid and password
void server_handle_not_found (void); // show 404 page
void server_handle_ping (void); // handle ping request

void server_handle_ajax (void); // handle ajax request
server_flags_t server_create_config (server_config_t *config);

#endif // SERVER_H