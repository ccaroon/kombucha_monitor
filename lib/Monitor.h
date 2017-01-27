#ifndef Monitor_h
#define Monitor_h

#include "DS18B20/DS18B20.h"
#include "DS18B20/Particle-OneWire.h"

struct Conditions {
    float tempF = 0.0;
    uint8_t brightness = 0;
};

class Monitor {
  public:
    Monitor();

    void begin();

    Conditions *getConditions();
    float getTempF();

  private:
    DS18B20 tempSensor = DS18B20(D2);

    Conditions data;
};

#endif
