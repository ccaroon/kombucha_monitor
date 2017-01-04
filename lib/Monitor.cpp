#include "Monitor.h"
#include <math.h>

// Initialize
Monitor::Monitor() {
}

void Monitor::begin() {
    // Init HTU21D
    htu21d.begin();

    // Init MPL3115A2
    mpl3115a2.begin();
    mpl3115a2.setModeBarometer();
    mpl3115a2.setOversampleRate(7);
    mpl3115a2.enableEventFlags();
}

// Housekeeping stuff
void Monitor::update() {
}

Conditions *Monitor::getConditions() {
    // Humidity from HTU21D
    data.humidity = htu21d.readHumidity();

    // Temperature from HTU21D (Fahrenheit)
    data.tempF = (htu21d.readTemperature() * 1.8) + 32.0;

    // Pressure from MPL3115A2 (Pascals)
    data.pressurePa = mpl3115a2.readPressure();

    update();

    return &data;
}
