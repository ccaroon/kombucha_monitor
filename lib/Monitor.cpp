#include "Monitor.h"
#include <math.h>

// Initialize
Monitor::Monitor() {
}

void Monitor::begin() {
    tempSensor.begin();
}

Conditions *Monitor::getConditions() {
    data.tempF = getTempF();

    // 0 to 4095 on Photon
    int val = analogRead(A0);
    // Map to percentage
    data.brightness = map(val, 0, 4095, 0, 100);
    return &data;
}

float Monitor::getTempF() {
    bool success = tempSensor.requestTemperaturesByIndex(0);

    float temp = data.tempF;
    if (success) {
        // NOTE: Although there is a getTempFByIndex() method and the code compiles
        // when using it, the resulting .bin file will not flash and work on
        // the Particle Photon for some reason. It puts it into "red blinking state"
        temp = (tempSensor.getTempCByIndex(0) * 1.8) + 32.0;
    }

    return (temp);
}
