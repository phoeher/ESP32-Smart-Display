#include "spotify_client.h"

#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "refresh.h"

String getBearerHeader(const String& accessToken) {
    return "Bearer " + accessToken;
}

String getCurrentlyPlaying(String& accessToken) {
    HTTPClient http;

    http.begin("https://api.spotify.com/v1/me/player/currently-playing");
    http.addHeader("Authorization", getBearerHeader(accessToken));

    int httpResponseCode = http.GET();
    String payload = http.getString();

    if (httpResponseCode == 401){

        Serial.println("Access token expired. Refreshing...");

        http.end();

        accessToken = refreshAccessToken();

        if (accessToken == ""){
            return "";
        }

        http.begin("https://api.spotify.com/v1/me/player/currently-playing");
        http.addHeader("Authorization", getBearerHeader(accessToken));

        httpResponseCode = http.GET();
        payload = http.getString();

    }

    if (httpResponseCode == 204) {
        http.end();
        return "";
    }

    if (httpResponseCode != 200) {

        Serial.println("Current track request failed.");
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

    String trackTitle = doc["item"]["name"];
    String artist = doc["item"]["artists"][0]["name"];
    String album = doc["item"]["album"]["name"];
    String albumArtUrl = doc["item"]["album"]["images"][0]["url"];

    bool isPlaying = doc["is_playing"];
    long progressMs = doc["progress_ms"];
    long durationMs = doc["item"]["duration_ms"];

    return trackTitle;
}