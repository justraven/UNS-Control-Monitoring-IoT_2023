/*
 * veml3235.cpp
 * Author : Muhammad Shofuwan Anwar
 * Created : 2023-12-20 00:21:00 
 */

#include "veml3235.h"

TwoWire veml3235_i2c = TwoWire(0);

uint16_t veml3235_init (void) {
    veml3235_i2c.begin(VEML3235_SDA_PIN, VEML3235_SCL_PIN, 400000);
    veml3235_set_config(VEML3235_DEFAULT_GAIN, VEML3235_DEFAULT_IT);
    vTaskDelay(4 / portTICK_PERIOD_MS);
    
    return veml3235_get_id();
}

veml3235_data_t veml3235_set_config (veml3235_gain_t gain, veml3235_it_t it) {
    veml3235_data_t config = {0};
    config.data8[0] = (0x00 | (it << 4));
    config.data8[1] = (0x01 | (gain << 3));

    veml3235_write(VEML3235_CONFIG, config);
    return veml3235_read(VEML3235_CONFIG);
}

float veml3235_get_white (void) {
    return ((float)veml3235_read(VEML3235_WHITE).data16 * LUX_COEFICIENT);
}

float veml3235_get_als (void) {
    return ((float)veml3235_read(VEML3235_ALS).data16 * LUX_COEFICIENT);
}

uint16_t veml3235_get_id (void) {
    return veml3235_read(VEML3235_ID).data16;
}

veml3235_data_t veml3235_read (uint8_t addr) {
    veml3235_data_t data = {0};

    veml3235_i2c.beginTransmission(VEML3235_I2C_ADDRESS);
    veml3235_i2c.write(addr);
    veml3235_i2c.endTransmission(false);

    veml3235_i2c.requestFrom(VEML3235_I2C_ADDRESS, 2);
    data.data8[0] = veml3235_i2c.read();
    data.data8[1] = veml3235_i2c.read();

    veml3235_i2c.endTransmission();

    return data;
}

void veml3235_write (uint8_t addr, veml3235_data_t data) {
    veml3235_i2c.beginTransmission(VEML3235_I2C_ADDRESS);
    veml3235_i2c.write(addr);
    veml3235_i2c.write(data.data8[0]);
    veml3235_i2c.write(data.data8[1]);
    veml3235_i2c.endTransmission();
}


