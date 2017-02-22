#include "Monitor.h"
#include "blynk.h"

#include <math.h>

#define UPDATE_INTERVAL 5 // seconds
#define LOW_TEMP 70.0
#define HIGH_TEMP 85.0
#define MAX_BREW_TIME 12.0 // days
#define DAY_IN_SECS 86400

// TODO:
// - start time to String
// - ability to set startTime from Blynk app

// Single Blynk App
char auth[] = "69f39e4c35fb424187209794d6a32264";

// Combined Blynk App (Omega)
// char auth[] = "551986437f03482bb5b8a7bbbc01623d";

// change startTime to human readable string
// Feb 20 2017 20:00
const long startTime = 1487638800;
const long completionTime = startTime + (DAY_IN_SECS * MAX_BREW_TIME);
const long totalBrewTime = completionTime - startTime;

long lastUpdate = 0;
Monitor monitor;

BLYNK_WRITE(V8) {
    if (param.asInt() == 1) {
        Blynk.virtualWrite(9, 255);
        // Particle.syncTime();
        Blynk.virtualWrite(9, 0);
    }
}

//------------------------------------------------------------------------------
void setup() {
    delay(5000);
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
        }

        Blynk.virtualWrite(2, data->brightness);

        Blynk.virtualWrite(7, durationToString(startTime, currTime));

        // ***** DEBUG ***** //
        Blynk.virtualWrite(20, Time.timeStr());
        // ***** DEBUG ***** //

        lastUpdate = currTime;

        // Turn off Virtual LED
        Blynk.virtualWrite(0, 0);
    }
}
