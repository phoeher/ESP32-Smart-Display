#include <Arduino.h>
#include <iostream>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "config.h"
#include "refresh.h"


// initialize variables here:

// put function declarations here:
void connectToWiFi();
void testInternetConnection();
//void fetchTrackData();

// put your setup code here, to run once:
void setup() {
  Serial.begin(11500);

  connectToWiFi();
  testInternetConnection();
  
  String accessToken = refreshAccessToken();

  Serial.println("Access token:");
  Serial.println(accessToken);
  
}

// put your main code here, to run repeatedly:
void loop() {
  //fetchTrackData();
  //delay(5000);
}

// put function definitions here:
void connectToWiFi() {
  Serial.println("connecting to WiFi...");

  // connect to WiFi network
  WiFi.begin(SSID, PASSWORD);

  // wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  // print the IP address
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
void testInternetConnection() {

  // test internet connection by making a GET request to a known website
  HTTPClient http;
  http.begin("https://api.ipify.org/");
  int httpResponseCode = http.GET();
  String payload = http.getString();
 
  Serial.println(payload);
  http.end();
}
/*
void fetchTrackData() {
  // make a GET request to the server to fetch the current track data
  HTTPClient http;
  http.begin("http://" + String(SERVER_IP) + ":5000/current-track");
  int httpResponseCode = http.GET();
  String payload = http.getString();

  // parse the JSON response and extract the track data
  JsonDocument doc;
  deserializeJson(doc, payload);

  String trackTitle = doc["track_title"];
  String artist1 = doc["artists"][0];
  String album = doc["album"];
  String isPlaying = doc["is_playing"];
  String progressMs = doc["progress_ms"];
  String durationMs = doc["duration_ms"];

  // print the track data to the serial monitor
  Serial.println(trackTitle);
  Serial.println(artist1);
  Serial.println(album);
  Serial.println(isPlaying);
  Serial.println(progressMs);
  Serial.println(durationMs);
  http.end();
}
*/