#ifndef DISPLAY_CLIENT_H
#define DISPLAY_CLIENT_H

#include <Arduino.h>

void initializeDisplay();
void showHelloWorld();
void clearDisplay();
void hibernateDisplay();

void showIdleScreen(
    const String& time,
    const String& date,
    const String& meridiem,
    float currentTemperature,
    float highTemperature,
    float lowTemperature,
    const String& condition,
    int precipitationProbability,
    float uvIndex,
    const unsigned char* icon
);

void updateTime(const String& time);

#endif
