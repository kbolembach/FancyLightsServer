#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "WiFiClient.h"
#include "SoftwareSerial.h"
#include <vector>

#include "server.h"
#include "globals.h"

// char *ssid = "***REMOVED***"; //Enter your WIFI ssid
// char *password = "***REMOVED***"; //Enter your WIFI password

// char *ssid = "***REMOVED***"; //Enter your WIFI ssid
// char *password = "***REMOVED***"; //Enter your WIFI password

char *ssid = "***REMOVED***"; //Enter your WIFI ssid
char *password = "***REMOVED***"; //Enter your WIFI password

IPAddress local_IP(192, 168, 0, 73);
IPAddress gateway(10, 32, 191, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(91, 90, 160, 4);
IPAddress secondaryDNS(91, 90, 160, 5);
ESP8266WebServer server(80);

constexpr unsigned int BAUD_RATE = 19200;
SoftwareSerial soft_serial(4, 5);

boolean new_data = false;
const byte num_chars = 32;
char received_chars[num_chars];

String send_data = "";
std::vector<String> recent_data = { "m01" };
String current_color = "#FF0000";
String received_data = "";

int brightness_value = 255;
String christmas_moving = "";

//function declarations
void recvWithStartEndMarkers();

void setup()
{
    Serial.begin(BAUD_RATE);
    soft_serial.begin(BAUD_RATE);
    delay(5000);
    Serial.println("ESP8266 ready - serial");
    soft_serial.println("*ESP8266 ready - soft_serial.");

    pinMode(LED_BUILTIN, OUTPUT);

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

    Serial.println("*WiFi connected");
    Serial.println("*IP address: ");
    Serial.println(WiFi.localIP());
    server.on( "/", handleRoot);
    server.begin();
    Serial.println("*HTTP server started");

    sendLineToArduino("It works!");

    setupHandlers();
}

void loop()
{
  recvWithStartEndMarkers();
  if (new_data == true)
  {
    String data(received_chars);
    Serial.println(data);

    new_data = false;
  }

  server.handleClient();
}

void recvWithStartEndMarkers(){
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '[';
    char endMarker = ']';
    char rc;
 
    while (soft_serial.available() > 0 && new_data == false) {
        rc = soft_serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                received_chars[ndx] = rc;
                ndx++;
                if (ndx >= num_chars) {
                    ndx = num_chars - 1;
                }
            }
            else {
                received_chars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                new_data = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}