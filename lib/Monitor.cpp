#include "Monitor.h"

// Initialize
Monitor::Monitor() {
}

void Monitor::begin() {
    pinMode(D2, INPUT);
}

Conditions *Monitor::getConditions() {
    data.tempF = getTempF();
    data.brightness = getBrightness();

    return &data;
}

int Monitor::getBrightness() {
    // 0 to 4095 on Photon
    int val = analogRead(A3);

    // Map to percentage
    return (map(val, 0, 4095, 0, 100));
}

float Monitor::getTempF() {
    float celsius, fahrenheit;
    int dsAttempts;

    fahrenheit = data.tempF;

    if (!tempSensor.search()) {
        tempSensor.resetsearch();
        celsius = tempSensor.getTemperature();
        while (!tempSensor.crcCheck() && dsAttempts < 4) {
            dsAttempts++;
            if (dsAttempts == 3) {
                delay(1000);
            }
            tempSensor.resetsearch();
            celsius = tempSensor.getTemperature();
            continue;
        }
        dsAttempts = 0;
        fahrenheit = tempSensor.convertToFahrenheit(celsius);
    }

    return (fahrenheit);
}
