#include "Monitor.h"
#include "blynk.h"
#include <math.h>

#define UPDATE_INTERVAL 5 // seconds

// Blynk Setup
char auth[] = "69f39e4c35fb424187209794d6a32264";

long startTime = 1482440400;
long lastUpdate = 0;
Monitor monitor;

//------------------------------------------------------------------------------
void setup() {
    delay(5000);
    Blynk.begin(auth);

    monitor.begin();

    Time.zone(-4);
}

//------------------------------------------------------------------------------
String durationToString(long start, long end) {

    // duration between start & end in seconds
    long duration = end - start;

    int days = duration / 86400;
    duration = duration % 86400;

    int hours = duration / 3600;
    duration = duration % 3600;

    int mins = duration / 60;
    int secs = duration % 60;

    char dStr[12];
    sprintf(dStr, "%02d:%02d:%02d:%02d", days, hours, mins, secs);

    return (String(dStr));
}
//------------------------------------------------------------------------------
void loop() {
    Blynk.run();

    // Publishes every UPDATE_INTERVAL seconds
    if (Time.now() - lastUpdate > UPDATE_INTERVAL) {
        // Turn on Virtual LED
        Blynk.virtualWrite(0, 255);

        Conditions *data = monitor.getConditions();

        Blynk.virtualWrite(1, data->tempF);
        Blynk.virtualWrite(2, data->humidity);

        String pressure = String::format("%0.2f", data->pressurePa * 0.0002953);
        Blynk.virtualWrite(3, pressure);

        if (data->tempF < 72.0) {
            Blynk.virtualWrite(4, 255);
            Blynk.virtualWrite(5, 0);
            Blynk.virtualWrite(6, 0);
        } else if (data->tempF > 85.0) {
            Blynk.virtualWrite(4, 0);
            Blynk.virtualWrite(5, 0);
            Blynk.virtualWrite(6, 255);
        } else {
            Blynk.virtualWrite(4, 0);
            Blynk.virtualWrite(5, 255);
            Blynk.virtualWrite(6, 0);
        }

        Blynk.virtualWrite(7, durationToString(startTime, Time.now()));

        lastUpdate = Time.now();

        // Turn off Virtual LED
        Blynk.virtualWrite(0, 0);
    }
}