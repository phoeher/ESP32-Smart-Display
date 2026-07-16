#include "weather_client.h"
#include "weather_icons.h"

#include <HTTPClient.h>
#include <ArduinoJson.h>

const float LATITUDE = 37.12;
const float LONGITUDE = -76.46;

String getWeatherCondition(int weatherCode);
const unsigned char* getConditionBitmap(const String& condition);


WeatherData getWeather() {

    String url = String("https://api.open-meteo.com/v1/forecast?") +
        "latitude=" + String(LATITUDE) +
        "&longitude=" + String(LONGITUDE);

    url += "&current=temperature_2m,weather_code";
    url += "&daily=temperature_2m_max,temperature_2m_min,precipitation_probability_max,uv_index_max";
    url += "&temperature_unit=fahrenheit";
    url += "&timezone=auto";

    HTTPClient http;
    http.begin(url);

    int httpResponseCode = http.GET();

    if (httpResponseCode != 200) {
        Serial.println("Weather request failed");
        http.end();
        return {};
    }

    String payload = http.getString();
    http.end();

    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        Serial.println("Weather JSON parsing failed");
        Serial.println(error.c_str());
        return {};
    }

    WeatherData weather;

    weather.currentTemperature = doc["current"]["temperature_2m"];
    doc["current"]["weather_code"];
    weather.weatherCode = getWeatherCondition(doc["current"]["weather_code"].as<int>());
    weather.highTemperature = doc["daily"]["temperature_2m_max"][0];
    weather.lowTemperature = doc["daily"]["temperature_2m_min"][0];
    weather.precipitationProbability = doc["daily"]["precipitation_probability_max"][0];
    weather.uvIndex = doc["daily"]["uv_index_max"][0];
    weather.icon = getConditionBitmap(weather.weatherCode);

    return weather;
}

String getWeatherCondition(int weatherCode){
    if (weatherCode == 0){
        return "Clear";
    }
    else if (weatherCode == 1 || weatherCode == 2){
        return "Partly Cloudy";
    }
    else if (weatherCode == 3){
        return "Overcast";
    }
    else if (weatherCode >= 45 && weatherCode <= 48){
        return "Fog";
    }
    else if (weatherCode >= 51 && weatherCode <= 57){
        return "Drizzle";
    }
    else if (weatherCode >= 61 && weatherCode <= 67){
        return "Rain";
    }
    else if (weatherCode >= 71 && weatherCode <= 77){
        return "Snow";
    }
    else if (weatherCode >= 80 && weatherCode <= 82){
        return "Rain showers";
    }
    else if (weatherCode >= 85 && weatherCode <= 86){
        return "Snow showers";
    }
    else if (weatherCode >= 95 && weatherCode <= 99){
        return "Thunderstorm";
    }
    else {
        return "Unknown";
    }
}

const unsigned char* getConditionBitmap(const String& condition) {
    if (condition == "Drizzle") {
        return epd_bitmap_allArray[0];
    }
    else if (condition == "Fog") {
        return epd_bitmap_allArray[1];
    }
    else if (condition == "Overcast") {
        return epd_bitmap_allArray[2];
    }
    else if (condition == "Partly Cloudy") {
        return epd_bitmap_allArray[3];
    }
    else if (condition == "Rain") {
        return epd_bitmap_allArray[4];
    }
    else if (condition == "Rain showers") {
        return epd_bitmap_allArray[5];
    }
    else if (condition == "Snow showers") {
        return epd_bitmap_allArray[6];
    }
    else if (condition == "Snow") {
        return epd_bitmap_allArray[7];
    }
    else if (condition == "Thunderstorm") {
        return epd_bitmap_allArray[8];
    }
    else if (condition == "Clear") {
        return epd_bitmap_allArray[9];
    }
    else{
        return nullptr;
    }
}
