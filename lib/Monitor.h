#ifndef Monitor_h
#define Monitor_h

#include "OneWire.h"
#include "application.h"
#include "spark-dallas-temperature.h"

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
    DallasTemperature tempSensor = DallasTemperature(new OneWire(D2));

    Conditions data;
};

#endif
