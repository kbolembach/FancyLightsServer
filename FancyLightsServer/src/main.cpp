#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "WiFiClient.h"
#include "SoftwareSerial.h"

const char *ssid = "***REMOVED***"; //Enter your WIFI ssid
const char *password = "***REMOVED***"; //Enter your WIFI password

// Static IP address
IPAddress local_IP(192, 168, 0, 73);
// Gateway IP address
//IPAddress gateway(192, 168, 1, 1);
IPAddress gateway(10, 32, 191, 1);

IPAddress subnet(255, 255, 0, 0);
//IPAddress primaryDNS(8, 8, 8, 8);   //optional
//IPAddress secondaryDNS(8, 8, 4, 4); //optional
IPAddress primaryDNS(91, 90, 160, 4);   //optional
IPAddress secondaryDNS(91, 90, 160, 5); //optional
ESP8266WebServer server(80);

SoftwareSerial softSerial(4, 5);

boolean newData = false;
const byte numChars = 32;
char receivedChars[numChars];

//function declarations
void recvWithStartEndMarkers();

void setup()
{
  Serial.begin(19200);
  softSerial.begin(19200);
  Serial.println("ESP8266 ready - serial");
  softSerial.println("ESP8266 ready - softSerial.");
}

void loop()
{
  recvWithStartEndMarkers();
  if (newData == true)
  {
    String data(receivedChars);
    Serial.println(data);

    newData = false;
  }

}

void recvWithStartEndMarkers(){
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '[';
    char endMarker = ']';
    char rc;
 
    while (softSerial.available() > 0 && newData == false) {
        rc = softSerial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}