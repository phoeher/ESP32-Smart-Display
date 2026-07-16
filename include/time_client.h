#ifndef TIME_CLIENT_H
#define TIME_CLIENT_H

#include <Arduino.h>

void initializeTime();
String getFormattedTime();
String getFormattedDate();
String getMeridiem();

#endif