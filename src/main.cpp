#include <Arduino.h>
#include <iostream>
#include <string>
#include <WiFi.h>
#include "secrets.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

// initialize variables here:

// put function declarations here:
void connectToWiFi();
void testInternetConnection();
void fetchTrackData();

// put your setup code here, to run once:
void setup() {
  Serial.begin(11500);

  connectToWiFi();
  testInternetConnection();
}

// put your main code here, to run repeatedly:
void loop() {
  fetchTrackData();
  delay(5000);
}

// put function definitions here:
void connectToWiFi() {
  Serial.println("connecting to WiFi...");
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
void testInternetConnection() {
  HTTPClient http;
  http.begin("https://api.ipify.org/");
  int httpResponseCode = http.GET();
  String payload = http.getString();
 
  Serial.println(payload);
  http.end();
}
void fetchTrackData() {
  HTTPClient http;
  http.begin("http://" + String(serverIP) + ":5000/current-track");
  int httpResponseCode = http.GET();
  String payload = http.getString();
  JsonDocument doc;
  deserializeJson(doc, payload);

  String trackTitle = doc["track_title"];
  String artist1 = doc["artists"][0];
  String album = doc["album"];
  String isPlaying = doc["is_playing"];
  String progressMs = doc["progress_ms"];
  String durationMs = doc["duration_ms"];

  Serial.println(trackTitle);
  Serial.println(artist1);
  Serial.println(album);
  Serial.println(isPlaying);
  Serial.println(progressMs);
  Serial.println(durationMs);
  http.end();
}