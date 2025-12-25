#ifdef IOT_H
#define IOT_H
#include <Arduino.h>
#include <PubsubClient.h>

#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#include "config.h"
#include "Espsensor.h"

Config config_network;
class wifi{
    private:

    public:
    bool connect_wifi(){}
    bool reconnect_Mqtt(){}
    bool callback(){}
    bool Publish_sensorData(){}
}

#endif