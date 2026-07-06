#include <Arduino.h>
#include <iostream>
#include <string>
#include <WiFi.h>
#include "secrets.h"
#include <HTTPClient.h>

// initialize variables here:

// put function declarations here:
void connectToWiFi();
void testInternetConnection();

// put your setup code here, to run once:
void setup() {
  Serial.begin(11500);

  connectToWiFi();
  testInternetConnection();
}

// put your main code here, to run repeatedly:
void loop() {
  //placeholder for main loop code
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
  http.begin("http://api.ipify.org");
  int httpResponseCode = http.GET();
  String payload = http.getString();
  Serial.println(payload);
  http.end();
}