#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_SHTC3.h>
#include "flags.h"
#include "veml3235.h"

#ifdef DEVICE_MONITOR_OLD
// #include <Adafruit_Sensor.h>
#include <DHT.h>
// #include <DHT_U.h>

#define DHT_PIN     32
#endif // DEVICE_MONITOR_OLD

#define SENSORS_SAMPLE_AVERAGE      50
#define SENSORS_TEMP_OFFSET         12.879f
#define SENSORS_HUMID_OFFSET        8.328f

#define SENSORS_SOIL_MOISTURE_PIN   39
#define SENSORS_SOIL_PH_PIN         36

typedef struct {
    float temperature;
    float humidity;
    float ambient_light;
    float white_light;
    float soil_moisture;
    float soil_ph;
} sensors_data_t;

void sensors_init (void);

sensors_data_t sensors_sample (void);
sensors_data_t sensors_get_data (void);

#endif // SENSORS_H