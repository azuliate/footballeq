 #include <esp_now.h>
#include <WiFi.h>

//Same struct as master
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

exercise myData;

//When the data is recieved
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);

  //Print all data to serial monitor
  Serial.print("nodeNum: ");
  Serial.println(myData.node);

  Serial.print("Phase 1 Colour: ");
  Serial.print(myData.phase1_colour);
  Serial.println();
  Serial.print("Phase 1 Seconds: ");
  Serial.println(myData.phase1_seconds);

  Serial.print("Phase 2 Colour: ");
  Serial.print(myData.phase2_colour);
  Serial.println();
  Serial.print("Phase 2 Seconds: ");
  Serial.println(myData.phase2_seconds);

  Serial.print("Phase 3 Colour: ");
  Serial.print(myData.phase3_colour);
  Serial.println();
  Serial.print("Phase 3 Seconds: ");
  Serial.println(myData.phase3_seconds);

  Serial.print("Phase 4 Colour: ");
  Serial.print(myData.phase4_colour);
  Serial.println();
  Serial.print("Phase 4 Seconds: ");
  Serial.println(myData.phase4_seconds);

  Serial.println();
}

void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {


}
