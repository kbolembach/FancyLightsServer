#include "server.h"
#include "globals.h"


void handleRoot() {
   server.send(200, "text/html", getWebpage());
}

String getWebpage(){
    String s = "(update 23:23, 24-01-26) color: " + current_color + ", brightness: " + brightness_value + ""
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
    "<form action=\"/CHRISTMAS_LIGHTS\" method=\"get\" id=\"form5\"></form><button type=\"submit\" form=\"form5\" value=\"Christmas lights\">Christmas lights</button>";
    return s;
}

void sendToArduino(String data){
    if (data[0] != '<'){
        data = '<' + data;
    }
    if (data[data.length() - 1] != '>'){
        data = data + '>';
    }
    char* csend_data = (char*) malloc(sizeof(char)*(data.length() + 1));
    send_data.toCharArray(csend_data, send_data.length() + 1);
    soft_serial.println(csend_data);
}