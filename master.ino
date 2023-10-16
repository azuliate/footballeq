// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid = "FOOTBALLEYEQ";
const char* password = "password";

String title;
String desc;
int exerciseNum = 0;
const String footballSymbol = "&#9917";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void evaluateExercise(){
  switch(exerciseNum){
    case 0:
      title = "";
      desc = "";
      break;
    case 1:
      title = "Exercise 1";
      desc = "Exercise 1 Description =D";
    default:
      break;
  }
}

String processor(const String& var){
  evaluateExercise();
  if(var == "TITLE"){
    return title;
  }
  if(var == "DESC"){
    return desc;
  }
  if(var == "FOOTBALL1"){
    if(exerciseNum == 1){
      return footballSymbol;
    } else{
      return "";
    }
  }
  if(var == "FOOTBALL2"){
    if(exerciseNum == 2){
      return footballSymbol;
    } else{
      return "";
    }
  }
  if(var == "FOOTBALL3"){
    if(exerciseNum == 3){
      return footballSymbol;
    } else{
      return "";
    }
  }
  if(var == "FOOTBALL4"){
    if(exerciseNum == 4){
      return footballSymbol;
    } else{
      return "";
    }
  }
  if(var == "FOOTBALL5"){
    if(exerciseNum == 5){
      return footballSymbol;
    } else{
      return "";
    }
  }
  
  return String();
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // host Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  //show IP address on serial output
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  

  //set wifi mode
  WiFi.mode(WIFI_AP_STA);

  //begin server
  server.begin();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/connect.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/connect.html", String(), false, processor);
  });

  server.on("/one", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 1;
    request->send(SPIFFS, "/connect.html", String(), false, processor);
  });

  server.on("/two", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 2;
    request->send(SPIFFS, "/connect.html", String(), false, processor);
  });

  server.on("/three", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 3;
    request->send(SPIFFS, "/connect.html", String(), false, processor);
  });

  server.on("/four", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 4;
    request->send(SPIFFS, "/connect.html", String(), false, processor);
  });

  server.on("/five", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 5;
    request->send(SPIFFS, "/connect.html", String(), false, processor);
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
