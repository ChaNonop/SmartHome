#include <Arduino.h>
#include "Sensor.h"


sensor::sensor(uint8_t pin_light, uint8_t buzzle_pin)
: _pin_light(pin_light), _buzzle_pin(buzzle_pin), dht(DHT)11)
{
    pinMode(_pin_light, INPUT);
    pinMode(_buzzle_pin, OUTPUT);
    dht.begin();
}