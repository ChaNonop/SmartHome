#ifndef ESPSENSOR_H
#define ESPSENSOR_H

class Sensor {
  public:
    Sensor(int pin);
    float readValue();
  private:
    int _DHTpin;
};

#endif
