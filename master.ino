/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

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

  // Start server
  server.begin();
}
 
void loop(){
  
}
