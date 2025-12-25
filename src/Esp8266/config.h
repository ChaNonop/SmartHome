#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

struct Config {
    const char* ssid = "";
    const char* password = "";

    const char* mqtt_server = "broker.netpie.io";
    const int mqtt_port = 1883;
    const char* mqtt_Client = "47eb28e0-0fe7-4eed-b90f-b86cd49baa38";
    const char* mqtt_username = "tVgNN3VMTFJzBEYoyj5pVEj23aM9Pc3c";
    const char* mqtt_password = "x6pR5J5fFmRHLDAonsigbBWozLd33oZpH";
};
#endif // CONFIG_H