/*
 * sensors.c
 * Author : Muhammad Shofuwan Anwar
 * Created : 2023-12-20 00:21:00
 */

#include "sensors.h"

Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();

void sensors_init (void) {
    if (!shtc3.begin()) {
        Serial.println("[ERROR] SHTC3 sensor not found.");
    }

    if (veml3235_init() != VEML3235_ID_VALUE) {
        Serial.println("[ERROR] VEML3235 sensor not found.");
    }
}

sensors_data_t sensors_sample (void) {
    sensors_data_t avg_data = {0};

    for (uint8_t i = 0; i < SENSORS_SAMPLE_AVERAGE; i++) {
        sensors_data_t sensors_data = sensors_get_data();
        avg_data.temperature += sensors_data.temperature;
        avg_data.humidity += sensors_data.humidity;
        avg_data.ambient_light += sensors_data.ambient_light;
        avg_data.white_light += sensors_data.white_light;
        avg_data.soil_moisture += sensors_data.soil_moisture;
        avg_data.soil_ph += sensors_data.soil_ph;
    }

    avg_data.temperature /= SENSORS_SAMPLE_AVERAGE;
    avg_data.humidity /= SENSORS_SAMPLE_AVERAGE;
    avg_data.ambient_light /= SENSORS_SAMPLE_AVERAGE;
    avg_data.white_light /= SENSORS_SAMPLE_AVERAGE;
    avg_data.soil_moisture /= SENSORS_SAMPLE_AVERAGE;
    avg_data.soil_ph /= SENSORS_SAMPLE_AVERAGE;

    return avg_data;
}

sensors_data_t sensors_get_data (void) {
    sensors_data_t sensors_data = {0};

    sensors_event_t humidity, temperature;
    shtc3.getEvent(&humidity, &temperature);

    sensors_data.temperature = temperature.temperature - SENSORS_TEMP_OFFSET;
    sensors_data.humidity = humidity.relative_humidity;

    sensors_data.ambient_light = veml3235_get_als();
    sensors_data.white_light = veml3235_get_white();

    sensors_data.soil_moisture = analogRead(SENSORS_SOIL_MOISTURE_PIN);
    sensors_data.soil_ph = analogRead(SENSORS_SOIL_PH_PIN);

    return sensors_data;
}


