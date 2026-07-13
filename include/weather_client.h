#ifndef WEATHER_CLIENT_H
#define WEATHER_CLIENT_H

#include <Arduino.h>

struct WeatherData {
    float currentTemperature = 0;
    float highTemperature = 0;
    float lowTemperature = 0;

    String weatherCode = "";
    
    int precipitationProbability = 0;
    float uvIndex = 0;
};

WeatherData getWeather();

#endif