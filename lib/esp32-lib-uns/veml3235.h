#ifndef VEMT3235_H
#define VEMT3235_H

#include <Arduino.h>
#include <Wire.h>

#define VEML3235_SDA_PIN        21
#define VEML3235_SCL_PIN        22
#define VEML3235_I2C_ADDRESS    0x10

#define VEML3235_DEFAULT_GAIN   VEML3235_GAIN_1
#define VEML3235_DEFAULT_IT     VEML3235_IT_100MS
#define VEML3235_ID_VALUE       0x135 // 0b0001 0011 0101

#define LUX_COEFICIENT          0.13632

typedef enum {
    VEML3235_GAIN_1 = 0b00,
    VEML3235_GAIN_2 = 0b01,
    VEML3235_GAIN_4 = 0b11,
} veml3235_gain_t;

typedef enum {
    VEML3235_IT_50MS = 0b000,
    VEML3235_IT_100MS = 0b001,
    VEML3235_IT_200MS = 0b010,
    VEML3235_IT_400MS = 0b011,
    VEML3235_IT_800MS = 0b100,
} veml3235_it_t;

typedef enum {
    VEML3235_CONFIG = 0x00,
    VEML3235_WHITE = 0x04,
    VEML3235_ALS = 0x05,
    VEML3235_ID = 0x09,
} veml3235_data_addr_t;

typedef union {
    uint8_t data8[2];
    uint16_t data16;
}veml3235_data_t;

uint16_t veml3235_init (void);

veml3235_data_t veml3235_set_config (veml3235_gain_t gain, veml3235_it_t it);

float veml3235_get_white (void);
float veml3235_get_als (void);

uint16_t veml3235_get_id (void);

veml3235_data_t veml3235_read (uint8_t addr);
void veml3235_write (uint8_t addr, veml3235_data_t data);

#endif // VEMT3235_H