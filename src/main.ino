#include "Monitor.h"
#include "blynk.h"

#include <math.h>

#define UPDATE_INTERVAL 5 // seconds
#define LOW_TEMP 70.0
#define HIGH_TEMP 85.0
#define MAX_BREW_TIME 12.0 // days
#define DAY_IN_SECS 86400
#define START_TIME_ADDR 0

// TODO:
// - ability to set startTime from Blynk app

// Single Blynk App
char auth[] = "69f39e4c35fb424187209794d6a32264";

//------------------------------------------------------------------------------
long lastUpdate = 0;
Monitor monitor;

long startTime = 0;
long completionTime = 0;
long totalBrewTime = 0;

// For tracking average temperature over time
unsigned long sampleCount = 0;
float tempSum = 0.0;

// Keeps track of which percent complete notification have been send
boolean notifyPercent[11] = {false};

//------------------------------------------------------------------------------
BLYNK_WRITE(V8) {
    if (param.asInt() == 1) {
        resetData(Time.now());
    }
}

BLYNK_WRITE(V10) {
    if (param.asInt() == 1) {
        // resetData(1488682800);
    }
}

//------------------------------------------------------------------------------
void notifyPercentComplete(float percent) {
    for (uint8_t p = 1; p <= 10; p++) {
        if (percent >= (p * 10) && notifyPercent[p] == false) {
            notifyPercent[p] = true;
            Blynk.notify(String::format("Kombucha %d%% Complete", p * 10));
        }
    }
}

//------------------------------------------------------------------------------
void setStartTime(long newTime) {
    startTime = newTime;
    completionTime = startTime + (DAY_IN_SECS * MAX_BREW_TIME);
    totalBrewTime = completionTime - startTime;

    EEPROM.put(START_TIME_ADDR, startTime);
}

//------------------------------------------------------------------------------
void resetData(long newTime) {
    setStartTime(newTime);

    // Avg Temp
    sampleCount = 0;
    tempSum = 0.0;

    // Notifications Sent
    memset(notifyPercent, false, sizeof(boolean) * 11);
}

//------------------------------------------------------------------------------
void setup() {
    delay(5000);

    EEPROM.get(START_TIME_ADDR, startTime);
    if (startTime == 0xFFFFFFFF) {
        // Mar 04 2017 22:00
        startTime = 1488682800;
    }
    setStartTime(startTime);

    Blynk.begin(auth);

    monitor.begin();

    Time.zone(-5);
}

//------------------------------------------------------------------------------
String durationToString(long start, long end) {

    // duration between start & end in seconds
    long duration = end - start;

    int days = duration / DAY_IN_SECS;
    duration = duration % DAY_IN_SECS;

    int hours = duration / 3600;
    duration = duration % 3600;

    int mins = duration / 60;
    int secs = duration % 60;

    char dStr[13];
    sprintf(dStr, "%02dd %02d:%02d:%02d", days, hours, mins, secs);

    return (String(dStr));
}

//------------------------------------------------------------------------------
void loop() {
    Blynk.run();

    long currTime = Time.now();

    // Update Blynk App every UPDATE_INTERVAL seconds
    if (currTime - lastUpdate >= UPDATE_INTERVAL) {
        // Turn on Virtual LED
        Blynk.virtualWrite(0, 255);

        Conditions *data = monitor.getConditions();

        long currBrewTime = currTime - startTime;
        float percentComplete =
            ((float)currBrewTime / (float)totalBrewTime) * 100.0;
        Blynk.virtualWrite(3, floor(percentComplete));
        notifyPercentComplete(percentComplete);

        // Get erratic reading from the Temp Sensor sometimes.
        // Protected against those
        if (data->tempF > 50.0) {
            Blynk.virtualWrite(1, String::format("%0.1f", data->tempF));

            if (data->tempF < LOW_TEMP) {
                Blynk.virtualWrite(4, 255);
                Blynk.virtualWrite(5, 0);
                Blynk.virtualWrite(6, 0);
            } else if (data->tempF > HIGH_TEMP) {
                Blynk.virtualWrite(4, 0);
                Blynk.virtualWrite(5, 0);
                Blynk.virtualWrite(6, 255);
            } else {
                Blynk.virtualWrite(4, 0);
                Blynk.virtualWrite(5, 255);
                Blynk.virtualWrite(6, 0);
            }

            sampleCount++;
            tempSum += data->tempF;
            Blynk.virtualWrite(
                9, String::format("%0.1f", tempSum / (float)sampleCount));
        }

        Blynk.virtualWrite(2, data->brightness);

        Blynk.virtualWrite(7, durationToString(startTime, currTime));

        lastUpdate = currTime;

        // Turn off Virtual LED
        Blynk.virtualWrite(0, 0);
    }
}
