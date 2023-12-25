#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>

#define ACTUATORS_RELAY0_PIN    33
#define ACTUATORS_RELAY1_PIN    25
#define ACTUATORS_SOLENOID0_PIN 32
#define ACTUATORS_SOLENOID1_PIN 26
#define ACTUATORS_SOLENOID2_PIN 27
#define ACTUATORS_SOLENOID3_PIN 34

typedef enum {
    ACTUATORS_RELAY0,
    ACTUATORS_RELAY1,
    ACTUATORS_SOLENOID0,
    ACTUATORS_SOLENOID1,
    ACTUATORS_SOLENOID2,
    ACTUATORS_SOLENOID3,

    ACTUATORS_NONE,
} actuators_t;

void actuators_init (void);
void actuators_set (actuators_t actuators, uint8_t state);

#endif // ACTUATORS_H