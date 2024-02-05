#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "WiFiClient.h"
#include "SoftwareSerial.h"
#include <vector>
#include <Preferences.h>

#include "server.h"
#include "globals.h"
#include "comm.h"

Preferences preferences;

String ssid;
String password;
boolean is_setup = false;
long login_request_time;

// char *ssid = "***REMOVED***"; //Enter your WIFI ssid
// char *password = "***REMOVED***"; //Enter your WIFI password

// char *ssid = "***REMOVED***"; //Enter your WIFI ssid
// char *password = "***REMOVED***"; //Enter your WIFI password

// char *ssid = "***REMOVED***"; //Enter your WIFI ssid
// char *password = "***REMOVED***"; //Enter your WIFI password

IPAddress local_IP(192, 168, 0, 73);
IPAddress gateway(10, 32, 191, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(91, 90, 160, 4);
IPAddress secondaryDNS(91, 90, 160, 5);
ESP8266WebServer server(80);

constexpr unsigned int BAUD_RATE = 19200;
SoftwareSerial soft_serial(4, 5); //rx, tx

boolean new_data = false;
const byte num_chars = 32;
char received_chars[num_chars];

String send_data = "";
std::vector<String> recent_data = { "m01" };
String current_color = "#FF0000";
String received_data = "";

int brightness_value = 255;
boolean christmas_moving;
LightMode light_mode;

void setup_wifi(){
    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        soft_serial.println("*STA Failed to configure");
    }
    Serial.println();
    Serial.print("Configuring access point...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    sendLineToArduino("<LS>"); //Logged in Succesfully
    Serial.println("*WiFi connected");
    Serial.println("*IP address: ");
    Serial.println(WiFi.localIP());
    server.on( "/", handleRoot);
    server.begin();
    Serial.println("*HTTP server started");

    sendLineToArduino("It works!");

    setupHandlers();
}

void setup()
{
    Serial.begin(BAUD_RATE);
    soft_serial.begin(BAUD_RATE);
    pinMode(LED_BUILTIN, OUTPUT);
    delay(5000);
    Serial.println("ESP8266 ready - serial");
    soft_serial.println("*ESP8266 ready - soft_serial.");

    preferences.begin("lights_server", false);
    ssid = preferences.getString("ssid", "");    
    password = preferences.getString("password", "");
    current_color = preferences.getString("color", current_color);
    brightness_value = preferences.getInt("brightness", 255);
    // christmas_moving = preferences.getBool("christmas_moving", false);
    light_mode = static_cast<LightMode>(preferences.getInt("light_mode", 0));


    if (ssid == "" || password == ""){
        sendLoginRequest();
        login_request_time = millis();
    }
    else {
        setup_wifi();
        is_setup = true;
    }
}

void loop()
{
    recvWithStartEndMarkers();
    if (new_data == true)
    {
        String data(received_chars);
        Serial.println("Data received: " + data);

        new_data = false;
        processData(data);

        if (password != "" && ssid != "" && !is_setup){
            setup_wifi();
            is_setup = true;
        }
    }

    if (is_setup){
        server.handleClient();
    }
    else {
        if (millis() - login_request_time >= 5000){
            login_request_time = millis();
            sendLoginRequest();
        }
    }
}