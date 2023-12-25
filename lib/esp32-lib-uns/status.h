#ifndef STATUS_H
#define STATUS_H

#include <Arduino.h>

#define STATUS_PIN_LED_BLUE     16
#define STATUS_PIN_LED_GREEN    17
#define STATUS_PIN_LED_RED      18

#define STATUS_DEFAULT_DELAY_MS 250

typedef enum {
    STATUS_WLAN_DISCONNECTED,
    STATUS_WLAN_CONNECTED,
    STATUS_WLAN_ERROR,
    STATUS_DATA_PUBLISHED,
    STATUS_DATA_ERROR,

    STATUS_NONE,
} status_t;

typedef enum {
    COLOR_NONE,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,
} color_t;

void status_init (void);
void status_rgb_set (color_t color);
void status_set (status_t status);
void status_blink (status_t status, uint8_t count, uint16_t delay_ms);

#endif // STATUS_H
