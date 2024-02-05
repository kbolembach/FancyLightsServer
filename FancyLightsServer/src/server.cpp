#include "server.h"
#include "globals.h"
#include "comm.h"
#include <vector>

void handleRoot() {
   server.send(200, "text/html", getWebpage());
}

String getWebpage(){
    String check = christmas_moving ? "checked" : "";
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
    "<input type=\"checkbox\" id=\"christmascheckbox\" form=\"form5\" name=\"christmascheckbox\" " + check + "  value=\"on\">"
    "<label for=\"christmascheckbox\">Moving?</label><br>"
    "</div>";
    return s;
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
        updateLights();
        light_mode = PLAIN;
        preferences.putInt("light_mode", static_cast<int>(light_mode));
        handleRoot();	
    });

    server.on("/CANDLE_FLICKER", []() {
        updateBrightness();
        updateLights();
        light_mode = CANDLE;
        preferences.putInt("light_mode", static_cast<int>(light_mode));
        handleRoot();
    });

    server.on("/CHRISTMAS_LIGHTS", []() {
        updateBrightness();
        updateMoving();
        updateLights();
        light_mode = CHRISTMAS;
        preferences.putInt("light_mode", static_cast<int>(light_mode));
        handleRoot();
    });
}

void updateLights(){
    String m;
    switch (light_mode){
        case PLAIN:
        recent_data.assign({ "m1", "c" + String(current_color), "b" + String(brightness_value) });
        Serial.println("Color changed to " + current_color + ", brighness to " + brightness_value + ".");
        break;

        case CANDLE:
        recent_data.assign({ "m2", "c" + String(current_color), "b" + String(brightness_value) });
        Serial.println("Candle flicker mode.");
        break;

        case CHRISTMAS:
        m = christmas_moving ? "1" : "0";
        recent_data.assign({ "m3" + m, "b" + String(brightness_value) });
        Serial.println("Christmas lights mode; brightness = " + String(brightness_value) + ".");
        break;

        default: break;
    }
    sendToArduino(recent_data);
}

void updateColor(){
    if (server.arg("color") == String("")) current_color = 0xFF0000; 
    else current_color = server.arg("color");
    preferences.putString("color", current_color);
}

void updateBrightness(){
    if (server.arg("brightness") == String("")) brightness_value = 255;
    else brightness_value = server.arg("brightness").toInt();
    preferences.putInt("brightness", brightness_value);
}

void updateMoving(){
    Serial.println("Checkbox: " + server.arg("christmascheckbox"));
    christmas_moving = server.arg("christmascheckbox") == "on" ? true : false;
    preferences.putBool("christmas_moving", christmas_moving);
}