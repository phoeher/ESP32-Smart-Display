# include "refresh.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <base64.h>
#include <ArduinoJson.h>

#include "config.h"

String getAuthHeader() {
    String credentials = String(CLIENT_ID) + ":" + String(CLIENT_SECRET);
    String encoded_credentials = base64::encode(credentials);
    return "Basic " + encoded_credentials;
}

String refreshAccessToken(){
    HTTPClient http;
    http.begin("https://accounts.spotify.com/api/token");
    http.addHeader("Authorization", getAuthHeader());
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String body = "grant_type=refresh_token&refresh_token=" + String(REFRESH_TOKEN);

    int httpResponseCode = http.POST(body);
    String payload = http.getString();

    if (httpResponseCode != 200){
        Serial.println("Token refresh failed");
        Serial.println("Status code: " + String(httpResponseCode));
        Serial.println("Response: " + payload);
        http.end();
        return "";
    }

    http.end();

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        Serial.println("JSON parsing failed");
        Serial.println(error.c_str());
        return "";
    }

    String accessToken = doc["access_token"];
    return accessToken;
}