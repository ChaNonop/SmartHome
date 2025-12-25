#include <Arduino.h>
#include "IOT.h"
#include "Espsensor.h"



wifi::connect_wifi(){
    WiFiManager wifiManager;
    wifiManager.autoConnect("IOT_House");
}