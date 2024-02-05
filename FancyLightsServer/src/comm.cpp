#include "server.h"
#include "globals.h"
#include "comm.h"
#include <vector>

void sendLoginRequest(){
    if (ssid == "" && password == "") sendLineToArduino("<W>");
    else if (ssid != "" && password == "") sendLineToArduino("<V>");
}

void processData(String data){
    switch (data[0]){
        case 'S':
        ssid = data.substring(1, data.length());
        Serial.println("SSID received: " + ssid);
        break;

        case 'P':
        password = data.substring(1, data.length());
        Serial.println("Password received: " + password);
        break;

        default:
        Serial.println("Uknown data: " + data);
        break;
    }
}

void sendLineToArduino(String line){
    // char* csend_data = (char*) malloc(sizeof(char)*(line.length() + 1));
    // send_data.toCharArray(csend_data, send_data.length() + 1);
    Serial.println(line);
    soft_serial.println(line);
}

void sendToArduino(std::vector<String> &data){
    int data_len = data.size() + 2;
    sendLineToArduino("<s" + String(data_len) + ">");
    delay(SEND_TIME);

    for (auto &s : data){
        if (s[0] != '<'){
            s = '<' + s;
        }
        if (s[s.length() - 1] != '>'){
            s = s + '>';
        }
        sendLineToArduino(s);
        delay(SEND_TIME);
    }
    
    sendLineToArduino("<e>");
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