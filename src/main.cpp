#include <Arduino.h>
#include "device_task.h"

void setup () {
    Serial.begin(115200);

    /* Device initialization */
    status_init();
    actuators_init();
    sensors_init();
    connections_init();
    Serial.println("[INFO] Device peripheral initialized.");

    /* Device task initialization */
    device_task_init();
    Serial.println("[INFO] Device task initialized.");

    for (uint8_t x=0;x<5;x++) {
        status_rgb_set(COLOR_GREEN);
        vTaskDelay(50 / portTICK_PERIOD_MS);
        status_rgb_set(COLOR_NONE);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void loop () {}


