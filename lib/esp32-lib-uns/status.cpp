/*
* status.c
* Author : Muhammad Shofuwan Anwar
* Created : 2023-12-20 00:21:00
*/

#include "status.h"

void status_init (void) {
    pinMode(STATUS_PIN_LED_RED, OUTPUT);
    pinMode(STATUS_PIN_LED_GREEN, OUTPUT);
    pinMode(STATUS_PIN_LED_BLUE, OUTPUT);

    digitalWrite(STATUS_PIN_LED_RED, LOW);
    digitalWrite(STATUS_PIN_LED_GREEN, LOW);
    digitalWrite(STATUS_PIN_LED_BLUE, LOW);
}

void status_rgb_set (color_t color) {
    const uint8_t color_map[8] = {
        // RGB
        0x00, // no status
        0x01, // red
        0x02, // green
        0x04, // blue
        0x03, // yellow
        0x05, // magenta
        0x06, // cyan
        0x07, // white
    };

    digitalWrite(STATUS_PIN_LED_RED, color_map[color] & 0x01);
    digitalWrite(STATUS_PIN_LED_GREEN, color_map[color] & 0x02);
    digitalWrite(STATUS_PIN_LED_BLUE, color_map[color] & 0x04);
}

void status_set (status_t status) {
    switch (status) {
        case STATUS_WLAN_DISCONNECTED:
            status_rgb_set(COLOR_MAGENTA);
            break;
        case STATUS_WLAN_CONNECTED:
            status_rgb_set(COLOR_GREEN);
            break;
        case STATUS_WLAN_ERROR:
            status_rgb_set(COLOR_RED);
            break;
        case STATUS_DATA_PUBLISHED:
            status_rgb_set(COLOR_CYAN);
            break;
        case STATUS_DATA_ERROR:
            status_rgb_set(COLOR_YELLOW);
            break;
        default:
            status_rgb_set(COLOR_NONE);
            break;
    }
}

void status_blink (status_t status, uint8_t count, uint16_t delay_ms) {
    for (uint8_t i = 0; i < count; i++) {
        status_set(status);
        vTaskDelay(delay_ms / portTICK_PERIOD_MS);
        status_set(STATUS_NONE);
        vTaskDelay(delay_ms / portTICK_PERIOD_MS);
    }
}


