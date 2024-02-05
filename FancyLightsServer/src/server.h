#pragma once
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "WiFiClient.h"
#include "SoftwareSerial.h"
#include <vector>

constexpr int SEND_TIME = 10;
void handleRoot();
String getWebpage();
void sendLineToArduino(String line);
void setupHandlers();
void updateColor();
void updateBrightness();
void updateMoving();
void updateLights();
void sendLoginRequest();
void recvWithStartEndMarkers();
void processData(String data);
void sendToArduino(std::vector<String> &data);