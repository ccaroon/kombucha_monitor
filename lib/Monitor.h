#include "application.h"

#ifndef Monitor_h
#define Monitor_h

#include "SparkFunHTU21D.h"
#include "SparkFunMPL3115A2.h"

struct Conditions {
    float humidity = 0.0;
    float tempF = 0.0;
    float pressurePa = 0.0;
};

class Monitor {
  public:
    // Constructor
    Monitor();

    void begin();
    void update();

    Conditions *getConditions();

  private:
    HTU21D htu21d;
    MPL3115A2 mpl3115a2;

    Conditions data;
};

#endif
