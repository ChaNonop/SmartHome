#ifndef SENSOR_H
#define SENSOR_H

#include <DHT.h>
#include <Arduino.h>

class sensor{
    private:
    const int _pin_light;
    const int _buzzle_pin;
    DHT11 dht11;
    DHT dht;

    public:
    sensor(uint8_t _pin_light, uint8_t _buzzle_pin);
    float read_temp();
    float read_humi();
}
#endif