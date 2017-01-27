#include "Monitor.h"
#include "blynk.h"

#define UPDATE_INTERVAL 5 // seconds
#define LOW_TEMP 70.0
#define HIGH_TEMP 85.0

// Single Blynk App
// char auth[] = "69f39e4c35fb424187209794d6a32264";

// Combined Blynk App (Omega)
char auth[] = "551986437f03482bb5b8a7bbbc01623d";

long startTime = 1484415000;
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

    // Update Blynk App every UPDATE_INTERVAL seconds
    if (Time.now() - lastUpdate > UPDATE_INTERVAL) {
        // Turn on Virtual LED
        Blynk.virtualWrite(0, 255);

        Conditions *data = monitor.getConditions();

        // Get erratic reading from the Temp Sensor sometimes.
        // Protected against those
        // if (data->tempF > 65.0) {
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
        // }

        Blynk.virtualWrite(2, data->brightness);

        Blynk.virtualWrite(7, durationToString(startTime, Time.now()));

        lastUpdate = Time.now();

        // Turn off Virtual LED
        Blynk.virtualWrite(0, 0);
    }
}
