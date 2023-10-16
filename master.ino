// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <esp_now.h>
#include <ArduinoJson.h>

typedef struct FeqData{
  int node;
  char phase1_colour[2];
  int phase1_seconds;
  char phase2_colour[2];
  int phase2_seconds;
  char phase3_colour[2];
  int phase3_seconds;
  char phase4_colour[2];
  int phase4_seconds;
} FeqData;

FeqData dataNode1;

// Replace with your network credentials
const char* ssid = "FOOTBALLEYEQ";
const char* password = "password";

String title;
String desc;
int exerciseNum;
const String footballSymbol = "&#9917";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

esp_now_peer_info_t peerInfo;

uint8_t node1[] = {0x34, 0x85, 0x18, 0x75, 0xE1, 0x24};

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void evaluateExercise(){
  switch(exerciseNum){
    case 0:
      title = "";
      desc = "";
      break;
    case 1:
      title = "Exercise 1";
      desc = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
      break;
    case 2:
      title = "Exercise 2";
      desc = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
      break;
    case 3:
      title = "Exercise 3";
      desc = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
      break;
    case 4:
      title = "Exercise 4";
      desc = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
      break;
    case 5:
      title = "Exercise 5";
      desc = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
      break;
    default:
      break;
  }
}

void parseExercise(){
  if(exerciseNum == 1){
    File exerciseFile = SPIFFS.open("/exercises/exercise1.json", "r");
      if(!exerciseFile){
        Serial.println("Failed to open config file");
        return;
      }

      size_t size = exerciseFile.size();

      //Allocate buffer to store json contents
      std::unique_ptr<char[]> buf(new char[size]);

      exerciseFile.readBytes(buf.get(), size);

      const size_t capacity = 16*JSON_ARRAY_SIZE(1) + 5*JSON_ARRAY_SIZE(4) + 17*JSON_OBJECT_SIZE(1) + 20*JSON_OBJECT_SIZE(2) + 590;
      DynamicJsonBuffer jsonBuffer(capacity);
      JsonObject& json = jsonBuffer.parseObject(buf.get());

      if(!json.success()){
        Serial.println("Failed to parse exercise file.");
        return;
      }

      dataNode1.node = json["nodes"][0]["node"]; // 1

  

      const char* node1_phase1_colour = json["nodes"][0]["phases"][0]["phase1"][0]["colour"];
      dataNode1.phase1_colour[0] = node1_phase1_colour[0];
      dataNode1.phase1_colour[1] = '\0';
      int node1_phase1_seconds = json["nodes"][0]["phases"][0]["phase1"][0]["seconds"];
      dataNode1.phase1_seconds = node1_phase1_seconds;

      const char* node1_phase2_colour = json["nodes"][0]["phases"][1]["phase2"][0]["colour"];
      dataNode1.phase2_colour[0] = node1_phase2_colour[0];
      dataNode1.phase2_colour[1] = '\0';
      int node1_phase2_seconds = json["nodes"][0]["phases"][1]["phase2"][0]["seconds"];
      dataNode1.phase2_seconds = node1_phase2_seconds;

      const char* node1_phase3_colour = json["nodes"][0]["phases"][2]["phase3"][0]["colour"];
      dataNode1.phase3_colour[0] = node1_phase3_colour[0];
      dataNode1.phase3_colour[1] = '\0';
      int node1_phase3_seconds = json["nodes"][0]["phases"][2]["phase3"][0]["seconds"];
      dataNode1.phase3_seconds = node1_phase3_seconds;

      const char* node1_phase4_colour = json["nodes"][0]["phases"][3]["phase4"][0]["colour"];
      dataNode1.phase4_colour[0] = node1_phase4_colour[0];
      dataNode1.phase4_colour[1] = '\0';
      int node1_phase4_seconds = json["nodes"][0]["phases"][3]["phase4"][0]["seconds"];
      dataNode1.phase4_seconds = node1_phase4_seconds;
      exerciseFile.close();
      return;
      } else{
        Serial.println("No exercise selected.");
      }
}
  

  /*int nodes_1_node = nodes[1]["node"]; // 2

  JsonArray& nodes_1_phases = nodes[1]["phases"];

  const char* nodes_1_phases_0_phase1_0_colour = nodes_1_phases[0]["phase1"][0]["colour"]; // "Red"
  int nodes_1_phases_0_phase1_0_seconds = nodes_1_phases[0]["phase1"][0]["seconds"]; // 10

  const char* nodes_1_phases_1_phase2_0_colour = nodes_1_phases[1]["phase2"][0]["colour"]; // "Green"
  int nodes_1_phases_1_phase2_0_seconds = nodes_1_phases[1]["phase2"][0]["seconds"]; // 5

  const char* nodes_1_phases_2_phase3_0_colour = nodes_1_phases[2]["phase3"][0]["colour"]; // "Red"
  int nodes_1_phases_2_phase3_0_seconds = nodes_1_phases[2]["phase3"][0]["seconds"]; // 5

  const char* nodes_1_phases_3_phase4_0_colour = nodes_1_phases[3]["phase4"][0]["colour"]; // "Green"
  int nodes_1_phases_3_phase4_0_seconds = nodes_1_phases[3]["phase4"][0]["seconds"]; // 5

  int nodes_2_node = nodes[2]["node"]; // 3

  JsonArray& nodes_2_phases = nodes[2]["phases"];

  const char* nodes_2_phases_0_phase1_0_colour = nodes_2_phases[0]["phase1"][0]["colour"]; // "Green"
  int nodes_2_phases_0_phase1_0_seconds = nodes_2_phases[0]["phase1"][0]["seconds"]; // 5

  const char* nodes_2_phases_1_phase2_0_colour = nodes_2_phases[1]["phase2"][0]["colour"]; // "Red"
  int nodes_2_phases_1_phase2_0_seconds = nodes_2_phases[1]["phase2"][0]["seconds"]; // 5

  const char* nodes_2_phases_2_phase3_0_colour = nodes_2_phases[2]["phase3"][0]["colour"]; // "Green"
  int nodes_2_phases_2_phase3_0_seconds = nodes_2_phases[2]["phase3"][0]["seconds"]; // 10

  const char* nodes_2_phases_3_phase4_0_colour = nodes_2_phases[3]["phase4"][0]["colour"]; // "Red"
  int nodes_2_phases_3_phase4_0_seconds = nodes_2_phases[3]["phase4"][0]["seconds"]; // 5

  int nodes_3_node = nodes[3]["node"]; // 4

  JsonArray& nodes_3_phases = nodes[3]["phases"];

  const char* nodes_3_phases_0_phase1_0_colour = nodes_3_phases[0]["phase1"][0]["colour"]; // "Green"
  int nodes_3_phases_0_phase1_0_seconds = nodes_3_phases[0]["phase1"][0]["seconds"]; // 5

  const char* nodes_3_phases_1_phase2_0_colour = nodes_3_phases[1]["phase2"][0]["colour"]; // "Red"
  int nodes_3_phases_1_phase2_0_seconds = nodes_3_phases[1]["phase2"][0]["seconds"]; // 5

  const char* nodes_3_phases_2_phase3_0_colour = nodes_3_phases[2]["phase3"][0]["colour"]; // "Green"
  int nodes_3_phases_2_phase3_0_seconds = nodes_3_phases[2]["phase3"][0]["seconds"]; // 10

  const char* nodes_3_phases_3_phase4_0_colour = nodes_3_phases[3]["phase4"][0]["colour"]; // "Red"
  int nodes_3_phases_3_phase4_0_seconds = nodes_3_phases[3]["phase4"][0]["seconds"]; // 5 */

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

  //initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //register callback function
  esp_now_register_send_cb(OnDataSent);

  //register peer
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  //register first peer
  memcpy(peerInfo.peer_addr, node1, 6);
   if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 1");
  }
  // register second peer  
  /*memcpy(peerInfo.peer_addr, node2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 2");
  }/*
  /// register third peer
  /*memcpy(peerInfo.peer_addr, node3, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 3");
  }*/
  

  //set wifi mode
  WiFi.mode(WIFI_AP_STA);

  //begin server
  server.begin();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 0;
    request->send(SPIFFS, "/webserver/index.html", "text/html");
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/webserver/style.css", "text/css");
  });

  server.on("/connect.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  server.on("/one", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 1;
    parseExercise();
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  server.on("/two", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 2;
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  server.on("/three", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 3;
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  server.on("/four", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 4;
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  server.on("/five", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 5;
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){
    esp_err_t result = esp_now_send(node1, (uint8_t *) &dataNode1, sizeof(FeqData));
      if (result == ESP_OK) {
        Serial.println("Sent with success");
      }
      else {
        Serial.println("Error sending the data");
      }
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
