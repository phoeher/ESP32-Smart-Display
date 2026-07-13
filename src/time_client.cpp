#include "time_client.h"

#include <time.h>

const char* NTP_SERVER = "pool.ntp.org";
const char* TIMEZONE = "EST5EDT,M3.2.0/2,M11.1.0/2";

void initializeTime() {
    configTzTime(TIMEZONE, NTP_SERVER);

    struct tm timeInfo;

    while (!getLocalTime(&timeInfo)){
        Serial.println("Waiting for time synchronization...");
        delay(500);
    }

    Serial.println("time sychronized.");


}

String getFormattedTime(){

    struct tm timeInfo;

    if (!getLocalTime(&timeInfo)){
        return "";
    }

    char timeString[20];

    strftime(
        timeString,
        sizeof(timeString),
        "%I:%M %p",
        &timeInfo
    );

    return String(timeString);
}

String getFormattedDate(){

    struct tm timeInfo;

    if (!getLocalTime(&timeInfo)){
        return "";
    }

    char dateString[30];

    strftime(
        dateString,
        sizeof(dateString),
        "%A, %B %d",
        &timeInfo
    );

    return String(dateString);
}