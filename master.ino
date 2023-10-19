// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <esp_now.h>
#include <ArduinoJson.h>

//Struct for data to be sent
typedef struct FeqData{
  boolean running;
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

//Instance of struct for each node
FeqData dataNode1;
FeqData dataNode2;
FeqData dataNode3;
FeqData dataNode4;

// Wifi network credentials
const char* ssid = "FOOTBALLEYEQ";
const char* password = "password";

//Initalizing variables
String title;
String desc;
int exerciseNum;
const String footballSymbol = "&#9917";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

esp_now_peer_info_t peerInfo;

//Storing node mac addresses
uint8_t node1[] = {0x34, 0x85, 0x18, 0x75, 0xE1, 0x24};
uint8_t node2[] = {0x34, 0x85, 0x18, 0x8D, 0x74, 0xB8};
uint8_t node3[] = {0x34, 0x85, 0x18, 0x96, 0xEF, 0x10};

//Logging if data has been sent
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

//Setting exercise
void evaluateExercise(){
  //Place holder exercises and descriptions
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
  //Opening the first exercise
  if(exerciseNum == 1){
    //Get the file
    File exerciseFile = SPIFFS.open("/exercises/exercise1.json", "r");
      
      if(!exerciseFile){
        Serial.println("Failed to open config file");
        return;
      }

      size_t size = exerciseFile.size();

      //Allocate buffer to store json contents
      std::unique_ptr<char[]> buf(new char[size]);

      //Reading in the file
      exerciseFile.readBytes(buf.get(), size);

      const size_t capacity = 16*JSON_ARRAY_SIZE(1) + 5*JSON_ARRAY_SIZE(4) + 17*JSON_OBJECT_SIZE(1) + 20*JSON_OBJECT_SIZE(2) + 590;
      DynamicJsonBuffer jsonBuffer(capacity);

      //Parsing the JSON
      JsonObject& json = jsonBuffer.parseObject(buf.get());

      if(!json.success()){
        Serial.println("Failed to parse exercise file.");
        return;
      }


      //Setting the data for the first node
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


      //Setting the data for the second node
      dataNode2.node = json["nodes"][1]["node"]; // 1

      const char* node2_phase1_colour = json["nodes"][1]["phases"][0]["phase1"][0]["colour"];
      dataNode2.phase1_colour[0] = node2_phase1_colour[0];
      dataNode2.phase1_colour[1] = '\0';
      int node2_phase1_seconds = json["nodes"][1]["phases"][0]["phase1"][0]["seconds"];
      dataNode2.phase1_seconds = node2_phase1_seconds;

      const char* node2_phase2_colour = json["nodes"][1]["phases"][1]["phase2"][0]["colour"];
      dataNode2.phase2_colour[0] = node2_phase2_colour[0];
      dataNode2.phase2_colour[1] = '\0';
      int node2_phase2_seconds = json["nodes"][1]["phases"][1]["phase2"][0]["seconds"];
      dataNode21.phase2_seconds = node2_phase2_seconds;

      const char* node2_phase3_colour = json["nodes"][1]["phases"][2]["phase3"][0]["colour"];
      dataNode2.phase3_colour[0] = node2_phase3_colour[0];
      dataNode2.phase3_colour[1] = '\0';
      int node2_phase3_seconds = json["nodes"][1]["phases"][2]["phase3"][0]["seconds"];
      dataNode2.phase3_seconds = node2_phase3_seconds;

      const char* node2_phase4_colour = json["nodes"][1]["phases"][3]["phase4"][0]["colour"];
      dataNode2.phase4_colour[0] = node2_phase4_colour[0];
      dataNode2.phase4_colour[1] = '\0';
      int node2_phase4_seconds = json["nodes"][1]["phases"][3]["phase4"][0]["seconds"];
      dataNode2.phase4_seconds = node2_phase4_seconds;

      //Setting the data for the third node
      dataNode3.node = json["nodes"][2]["node"]; // 1

      const char* node3_phase1_colour = json["nodes"][2]["phases"][0]["phase1"][0]["colour"];
      dataNode3.phase1_colour[0] = node3_phase1_colour[0];
      dataNode3.phase1_colour[1] = '\0';
      int node3_phase1_seconds = json["nodes"][2]["phases"][0]["phase1"][0]["seconds"];
      dataNode3.phase1_seconds = node3_phase1_seconds;

      const char* node3_phase2_colour = json["nodes"][2]["phases"][1]["phase2"][0]["colour"];
      dataNode3.phase2_colour[0] = node3_phase2_colour[0];
      dataNode3.phase2_colour[1] = '\0';
      int node3_phase2_seconds = json["nodes"][2]["phases"][1]["phase2"][0]["seconds"];
      dataNode3.phase2_seconds = node3_phase2_seconds;

      const char* node3_phase3_colour = json["nodes"][2]["phases"][2]["phase3"][0]["colour"];
      dataNode3.phase3_colour[0] = node3_phase3_colour[0];
      dataNode3.phase3_colour[1] = '\0';
      int node3_phase3_seconds = json["nodes"][2]["phases"][2]["phase3"][0]["seconds"];
      dataNode3.phase3_seconds = node3_phase3_seconds;

      const char* node3_phase4_colour = json["nodes"][2]["phases"][3]["phase4"][0]["colour"];
      dataNode3.phase4_colour[0] = node3_phase4_colour[0];
      dataNode3.phase4_colour[1] = '\0';
      int node3_phase4_seconds = json["nodes"][2]["phases"][3]["phase4"][0]["seconds"];
      dataNode3.phase4_seconds = node3_phase4_seconds;

      //Setting own data
      dataNode4.node = json["nodes"][3]["node"]; // 1

      const char* node4_phase1_colour = json["nodes"][3]["phases"][0]["phase1"][0]["colour"];
      dataNode4.phase1_colour[0] = node4_phase1_colour[0];
      dataNode4.phase1_colour[1] = '\0';
      int node4_phase1_seconds = json["nodes"][3]["phases"][0]["phase1"][0]["seconds"];
      dataNode4.phase1_seconds = node4_phase1_seconds;

      const char* node4_phase2_colour = json["nodes"][3]["phases"][1]["phase2"][0]["colour"];
      dataNode4.phase2_colour[0] = node4_phase2_colour[0];
      dataNode4.phase2_colour[1] = '\0';
      int node4_phase2_seconds = json["nodes"][3]["phases"][1]["phase2"][0]["seconds"];
      dataNode4.phase2_seconds = node4_phase2_seconds;

      const char* node4_phase3_colour = json["nodes"][3]["phases"][2]["phase3"][0]["colour"];
      dataNode4.phase3_colour[0] = node4_phase3_colour[0];
      dataNode4.phase3_colour[1] = '\0';
      int node4_phase3_seconds = json["nodes"][3]["phases"][2]["phase3"][0]["seconds"];
      dataNode4.phase3_seconds = node4_phase3_seconds;

      const char* node4_phase4_colour = json["nodes"][3]["phases"][3]["phase4"][0]["colour"];
      dataNode4.phase4_colour[0] = node4_phase4_colour[0];
      dataNode4.phase4_colour[1] = '\0';
      int node4_phase4_seconds = json["nodes"][3]["phases"][3]["phase4"][0]["seconds"];
      dataNode4.phase4_seconds = node4_phase4_seconds;
      
      exerciseFile.close();
      return;
      } else{
        Serial.println("No exercise selected.");
      }
}

//Method for replacing placeholder strings on the website
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
  memcpy(peerInfo.peer_addr, node2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 2");
  }
  /// register third peer
  memcpy(peerInfo.peer_addr, node3, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 3");
  }
  

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

  // Route for connect screen
  server.on("/connect.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  //Route for exercise one selected
  server.on("/one", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 1;
    parseExercise();
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  //Route for exercise two selected
  server.on("/two", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 2;
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  //Route for exercise three selected
  server.on("/three", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 3;
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  //Route for exercise four selected
  server.on("/four", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 4;
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  //Route for exercise five selected
  server.on("/five", HTTP_GET, [](AsyncWebServerRequest *request){
    exerciseNum = 5;
    request->send(SPIFFS, "/webserver/connect.html", String(), false, processor);
  });

  //Route for start button
  server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){
    //Sending the data
    esp_err_t result = esp_now_send(node1, (uint8_t *) &dataNode1, sizeof(FeqData));
    esp_err_t result1 = esp_now_send(node2, (uint8_t *) &dataNode2, sizeof(FeqData));
    esp_err_t result2 = esp_now_send(node3, (uint8_t *) &dataNode3, sizeof(FeqData));
      if (result == ESP_OK && result1 == ESP_OK && result2 == ESP_OK) {
        Serial.println("All data sent");
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
