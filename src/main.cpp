#include <Arduino.h>
#include <iostream>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "config.h"
#include "refresh.h"
#include "spotify_client.h"
#include "time_client.h"
#include "weather_client.h"
#include "display_client.h"

// initialize variables here:
String accessToken = "";
SpotifyTrack trackInfo;

// put function declarations here:
void connectToWiFi();
void testInternetConnection();
void fetchTrackData();

// put your setup code here, to run once:
void setup() {
  Serial.begin(11500);

  connectToWiFi();
  testInternetConnection();
  initializeTime();
  getWeather();  Serial.println("initializing display...");
  initializeDisplay();
  //showHelloWorld();
  clearDisplay();
  hibernateDisplay();
  
  accessToken = refreshAccessToken();
  
}

// put your main code here, to run repeatedly:
void loop() {
  Serial.println(getFormattedTime());
  Serial.println(getFormattedDate());
  fetchTrackData();
  delay(5000);
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

void fetchTrackData() {

  // get track data
  trackInfo = getCurrentlyPlaying(accessToken);
  
  // check if track is playing
  if (trackInfo.title == ""){
    Serial.println("No track playing");
  }
  //print track data
  else{
    Serial.println(trackInfo.title);
    Serial.println(trackInfo.artist);
    Serial.println(trackInfo.album);
    Serial.println(trackInfo.albumArtUrl);

    Serial.println(trackInfo.isPlaying);

    Serial.println(trackInfo.progressMs);
    Serial.println(trackInfo.durationMs);
  }
  
}