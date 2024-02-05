#pragma once
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "WiFiClient.h"
#include "SoftwareSerial.h"
#include <vector>

void sendLoginRequest();
void processData(String data);
void sendLineToArduino(String line);
void sendToArduino(std::vector<String> &data);
void recvWithStartEndMarkers();