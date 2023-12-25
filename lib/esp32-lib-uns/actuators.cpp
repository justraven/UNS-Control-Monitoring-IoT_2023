/*
* actuators.c
* Author : Muhammad Shofuwan Anwar
* Created : 2023-12-20 00:21:00
*/

#include "actuators.h"

const uint8_t __actuators_pins[] = {
    ACTUATORS_RELAY0_PIN,
    ACTUATORS_RELAY1_PIN,
    ACTUATORS_SOLENOID0_PIN,
    ACTUATORS_SOLENOID1_PIN,
    ACTUATORS_SOLENOID2_PIN,
    ACTUATORS_SOLENOID3_PIN,
};

void actuators_init (void) {
    for (uint8_t i = 0; i < ACTUATORS_NONE; i++) {
        pinMode(__actuators_pins[i], OUTPUT);
    }
}

void actuators_set (actuators_t actuators, uint8_t state) {
    digitalWrite(__actuators_pins[actuators], state);
}


