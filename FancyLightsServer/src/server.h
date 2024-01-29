#pragma once
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "WiFiClient.h"
#include "SoftwareSerial.h"

void handleRoot();
String getWebpage();
void sendToArduino(String data);