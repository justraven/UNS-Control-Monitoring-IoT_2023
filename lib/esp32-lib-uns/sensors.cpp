/*
 * sensors.c
 * Author : Muhammad Shofuwan Anwar
 * Created : 2023-12-20 00:21:00
 */

#include "sensors.h"

Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();

#ifdef DEVICE_MONITOR_OLD
DHT dht(DHT_PIN, DHT22);
#endif // DEVICE_MONITOR_OLD

void sensors_init (void) {
#if !defined(DEVICE_SOIL_MONITOR0) && !defined(DEVICE_SOIL_MONITOR1) && !defined(DEVICE_SOIL_MONITOR2) && !defined(DEVICE_SOIL_MONITOR3) &&\
    !defined(DEVICE_SOIL_MONITOR4) && !defined(DEVICE_SOIL_MONITOR5) && !defined(DEVICE_SOIL_MONITOR6) && !defined(DEVICE_SOIL_MONITOR7)\
    && !defined(DEVICE_SOIL_MONITOR8) && !defined(DEVICE_CONTROLLER_OUTSIDE) && !defined(DEVICE_MONITOR_OLD)
    if (!shtc3.begin()) {
        Serial.println("[ERROR] SHTC3 sensor not found.");
    }

    if (veml3235_init() != VEML3235_ID_VALUE) {
        Serial.println("[ERROR] VEML3235 sensor not found.");
    }
#elif defined(DEVICE_MONITOR_OLD)
    dht.begin();
#endif
}

sensors_data_t sensors_sample (void) {
#ifndef DEVICE_MONITOR_OLD
    sensors_data_t sensors_avg = {0.0f};

    for (uint8_t i = 0; i < SENSORS_SAMPLE_AVERAGE; i++) {
        sensors_data_t sensors_data = sensors_get_data();
        
        sensors_avg.temperature += sensors_data.temperature;
        sensors_avg.humidity += sensors_data.humidity;
        sensors_avg.ambient_light += sensors_data.ambient_light;
        sensors_avg.white_light += sensors_data.white_light;
        sensors_avg.soil_moisture += sensors_data.soil_moisture;
        sensors_avg.soil_ph += sensors_data.soil_ph;
    }

    sensors_avg.temperature /= SENSORS_SAMPLE_AVERAGE;
    sensors_avg.humidity /= SENSORS_SAMPLE_AVERAGE;
    sensors_avg.ambient_light /= SENSORS_SAMPLE_AVERAGE;
    sensors_avg.white_light /= SENSORS_SAMPLE_AVERAGE;
    sensors_avg.soil_moisture /= SENSORS_SAMPLE_AVERAGE;
    sensors_avg.soil_ph /= SENSORS_SAMPLE_AVERAGE;
#elif defined(DEVICE_MONITOR_OLD)
    sensors_data_t sensors_median[10] = {0.0f};
    sensors_data_t sensors_avg = {0.0f};

    for (uint8_t i = 0; i < 10; i++) {
        sensors_median[i].temperature = dht.readTemperature() - FLAGS_DHT_OFFSET_TEMP;
        sensors_median[i].humidity = dht.readHumidity() + FLAGS_DHT_OFFSET_HUMID;
    }

    // sort temperature
    for (uint8_t i = 0; i < 10; i++) {
        for (uint8_t j = i + 1; j < 10; j++) {
            if (sensors_median[i].temperature > sensors_median[j].temperature) {
                float temp = sensors_median[i].temperature;
                sensors_median[i].temperature = sensors_median[j].temperature;
                sensors_median[j].temperature = temp;
            }
        }
    }

    // sort humidity
    for (uint8_t i = 0; i < 10; i++) {
        for (uint8_t j = i + 1; j < 10; j++) {
            if (sensors_median[i].humidity > sensors_median[j].humidity) {
                float temp = sensors_median[i].humidity;
                sensors_median[i].humidity = sensors_median[j].humidity;
                sensors_median[j].humidity = temp;
            }
        }
    }

    // get median
    sensors_avg.temperature = sensors_median[5].temperature;
    sensors_avg.humidity = sensors_median[5].humidity;
#endif

    return sensors_avg;
}

sensors_data_t sensors_get_data (void) {
    sensors_data_t sensors_data = {0};

#if !defined(DEVICE_SOIL_MONITOR0) && !defined(DEVICE_SOIL_MONITOR1) && !defined(DEVICE_SOIL_MONITOR2) && !defined(DEVICE_SOIL_MONITOR3) &&\
    !defined(DEVICE_SOIL_MONITOR4) && !defined(DEVICE_SOIL_MONITOR5) && !defined(DEVICE_SOIL_MONITOR6) && !defined(DEVICE_SOIL_MONITOR7)\
    && !defined(DEVICE_SOIL_MONITOR8) && !defined(DEVICE_CONTROLLER_OUTSIDE) && !defined(DEVICE_MONITOR_OLD)
    sensors_event_t humidity, temperature;
    shtc3.getEvent(&humidity, &temperature);

    sensors_data.temperature = temperature.temperature - SENSORS_TEMP_OFFSET;
    sensors_data.humidity = humidity.relative_humidity + SENSORS_HUMID_OFFSET;

    sensors_data.ambient_light = veml3235_get_als();
    sensors_data.white_light = veml3235_get_white();
#endif

#if defined(DEVICE_SOIL_MONITOR0) || defined(DEVICE_SOIL_MONITOR1) || defined(DEVICE_SOIL_MONITOR2) || defined(DEVICE_SOIL_MONITOR3) ||\
    defined(DEVICE_SOIL_MONITOR4) || defined(DEVICE_SOIL_MONITOR5) || defined(DEVICE_SOIL_MONITOR6) || defined(DEVICE_SOIL_MONITOR7)\
    || defined(DEVICE_SOIL_MONITOR8)
    float soil_ph_value = 0.0f;

    switch (analogRead(SENSORS_SOIL_MOISTURE_PIN)) {
        case 0 ... 173:
            soil_ph_value = 10.0f;
        break;

        case 174 ... 572:
            soil_ph_value = 9.0f;
        break;

        case 573 ... 889:
            soil_ph_value = 8.0f;
        break;

        case 890 ... 1073:
            soil_ph_value = 7.0f;
        break;

        case 1074 ... 1257:
            soil_ph_value = 6.0f;
        break;

        case 1258 ... 1441:
            soil_ph_value = 5.0f;
        break;

        case 1442 ... 1625:
            soil_ph_value = 4.0f;
        break;

        case 1626 ... 4095:
            soil_ph_value = 3.0f;
        break;

        default:
            soil_ph_value = 0.0f;
        break;
    }
    
    sensors_data.soil_ph = soil_ph_value;
    sensors_data.soil_moisture = ((4095.0f - analogRead(SENSORS_SOIL_MOISTURE_PIN))/4095.0f)*100.0f;
    // sensors_data.soil_moisture = 3.89f;
    // sensors_data.soil_ph = 3.4f;
#endif

    return sensors_data;
}


