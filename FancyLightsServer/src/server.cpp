#include "server.h"
#include "globals.h"
#include <vector>

void handleRoot() {
   server.send(200, "text/html", getWebpage());
}

String getWebpage(){
    String s = "(update 19:15, 03-02-24) color: " + current_color + ", brightness: " + brightness_value + ""
    "<form action=\"/LED_BUILTIN_on\" method=\"get\" id=\"form1\"></form><button type=\"submit\" form=\"form1\" value=\"On\">On</button>"
    "<form action=\"/LED_BUILTIN_off\" method=\"get\" id=\"form2\"></form><button type=\"submit\" form=\"form2\" value=\"Off\">Off</button>"
    "<form action=\"/CHANGE_COLOR\" method=\"get\" id=\"form3\">"
    "<label for=\"colorpicker\">Color Picker:</label>"
    "<input type=\"color\" id=\"colorpicker\" name=\"color\" value=\"" + current_color + "\">"
    "<div>"
    "<input type=\"range\" id=\"brightness\" name=\"brightness\" min=\"0\" max=\"255\" value=\"" + brightness_value + "\">"
    "<label for=\"brightness\">Brightness: " + brightness_value + "</label>"
    "</div>"
    "</form><button type=\"submit\" form=\"form3\" value=\"Plain color\">Plain color</button>"
    "<form action=\"/CANDLE_FLICKER\" method=\"get\" id=\"form4\"></form><button type=\"submit\" form=\"form4\" value=\"Candle flicker\">Candle flicker</button>"
    "<div>"
    "<form action=\"/CHRISTMAS_LIGHTS\" method=\"get\" id=\"form5\"></form><button type=\"submit\" form=\"form5\" value=\"Christmas lights\">Christmas lights</button>"
    "<input type=\"checkbox\" id=\"christmascheckbox\" name=\"christmascheckbox\" value=\"" + christmas_moving + "\">"
    "<label for=\"christmascheckbox\">Moving?</label><br>"
    "</div>";
    return s;
}

void sendLineToArduino(String line){
    // char* csend_data = (char*) malloc(sizeof(char)*(line.length() + 1));
    // send_data.toCharArray(csend_data, send_data.length() + 1);
    Serial.println(line);
    soft_serial.println(line);
}

void setupHandlers(){
        server.on("/LED_BUILTIN_on", []() {
        digitalWrite(LED_BUILTIN, LOW);
        recent_data.assign({ "m01" });
        sendToArduino(recent_data);
        Serial.println("LED on.");
        handleRoot();
    });

    server.on("/LED_BUILTIN_off", []() {
        digitalWrite(LED_BUILTIN, HIGH);
        recent_data.assign({ "m00" });
        sendToArduino(recent_data);
        Serial.println("LED off.");
        handleRoot();
    });

    server.on("/CHANGE_COLOR", []() {
        updateBrightness();
        updateColor();
        recent_data.assign({ "m1", "c" + String(current_color), "b" + String(brightness_value) });
        sendToArduino(recent_data);
        Serial.println("Color changed to " + current_color + ", brighness to " + brightness_value + ".");
        handleRoot();	
    });

    server.on("/CANDLE_FLICKER", []() {
        updateBrightness();
        recent_data.assign({ "m2", "c" + String(current_color), "b" + String(brightness_value) });
        sendToArduino(recent_data);
        Serial.println("Candle flicker mode.");
        handleRoot();
    });

    server.on("/CHRISTMAS_LIGHTS", []() {
        updateBrightness();
        updateMoving();
        String m = christmas_moving ? "1" : "0";
        recent_data.assign({ "m3" + m, "b" + String(brightness_value) });
        sendToArduino(recent_data);
        Serial.println("Christmas lights mode; brightness = " + String(brightness_value) + ".");
        handleRoot();
    });
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

void updateColor(){
    if (server.arg("color") == String("")) current_color = 0xFF0000; 
    else current_color = server.arg("color");
}

void updateBrightness(){
    if (server.arg("brightness") == String("")) brightness_value = 255;
    else brightness_value = server.arg("brightness").toInt();
}

void updateMoving(){
    if (server.arg("christmascheckbox") == String("")) christmas_moving = "";
    else christmas_moving = server.arg("christmascheckbox");
}