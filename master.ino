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


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

 
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

  // Start server
  server.begin();
}
 
void loop(){
  
}
