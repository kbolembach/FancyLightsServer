#pragma once
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "WiFiClient.h"
#include "SoftwareSerial.h"
#include <vector>

extern char *ssid; //Enter your WIFI ssid
extern char *password; //Enter your WIFI password

extern IPAddress local_IP;
extern IPAddress gateway;

extern IPAddress subnet;
extern IPAddress primaryDNS;
extern IPAddress secondaryDNS;
extern ESP8266WebServer server;

extern SoftwareSerial soft_serial;

extern boolean new_data;
extern const byte num_chars;
extern char received_chars[];

extern String send_data;
extern std::vector<String> recent_data;
extern String current_color;
extern String received_data;

extern int brightness_value;
extern String christmas_moving;