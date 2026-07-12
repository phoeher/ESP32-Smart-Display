#include "spotify_client.h"

#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "refresh.h"

String getBearerHeader(const String& accessToken) {
    return "Bearer " + accessToken;
}

SpotifyTrack getCurrentlyPlaying(String& accessToken) {
    // create empty struct
    SpotifyTrack track;

    // create http object
    HTTPClient http;

    // begin call to spotify
    http.begin("https://api.spotify.com/v1/me/player/currently-playing");
    http.setTimeout(5000);
    http.addHeader("Authorization", getBearerHeader(accessToken));

    // get response code
    int httpResponseCode = http.GET();

    // check for no track playing code
    if (httpResponseCode == 204) {
        http.end();
        return {};
    }

    // get response string
    String payload = http.getString();

    // check for access code expiration
    if (httpResponseCode == 401){

        Serial.println("Access token expired. Refreshing...");

        http.end();

        accessToken = refreshAccessToken();

        if (accessToken == ""){
            return {};
        }

        http.begin("https://api.spotify.com/v1/me/player/currently-playing");
        http.setTimeout(5000);
        http.addHeader("Authorization", getBearerHeader(accessToken));

        httpResponseCode = http.GET();
        payload = http.getString();

    }

    // check for all other errors
    if (httpResponseCode != 200) {

        Serial.println("Current track request failed.");
        Serial.println("Status code: " + String(httpResponseCode));
        Serial.println("Response: " + payload);

        http.end();
        return {};
    }

    http.end();

    // create doc object
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    // check for parsing error
    if (error) {
        Serial.println("JSON parsing failed");
        Serial.println(error.c_str());
        return {};
    }

    // assign struct variables to track information
    track.title = doc["item"]["name"].as<String>();
    track.artist = doc["item"]["artists"][0]["name"].as<String>();
    track.album = doc["item"]["album"]["name"].as<String>();
    track.albumArtUrl = doc["item"]["album"]["images"][0]["url"].as<String>();

    track.isPlaying = doc["is_playing"].as<bool>();
    track.progressMs = doc["progress_ms"].as<long>();
    track.durationMs = doc["item"]["duration_ms"].as<long>();

    return track;
}