/**
 * ESP32 Toolkit Template
 * 
 * Embedded Features:
 * - Wifi Network: AP + STA
 * - Embedded Filesystem: SPIFFS
 * - Webservices: Webserver, Websockets 
 * - OTA updates
 * - Power management: sleep/wake cycle
**/

#include <iostream>

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
// #include <Arduino_JSON.h>
#include <AsyncElegantOTA.h>

const char *hotspotSSID = "******";
const char *hotspotPWD = "******";

const char *wifiSSID = "******";
const char *wifiPWD = "******";

// using namespace std;

AsyncWebServer server(80);

AsyncWebSocket ws("/ws");


/** 
 * FileSystem 
 **/
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

/** 
 * Network 
 * - Access Point Mode
 * - Client mode
**/
void WifiAP()
{
  Serial.print("Setting AP (Access Point)...");
  WiFi.softAP(hotspotSSID, hotspotPWD);
  std::cout << "Setup with IP " << WiFi.softAPIP() << std::endl;
}

void WifiSTA()
{
  WiFi.begin(wifiSSID, wifiPWD);
  std::cout << "Connecting to " << wifiSSID << " network..." << std::endl;

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  std::cout << std::endl
       << "Connected with IP address: " << WiFi.localIP() << std::endl;
}

/** 
 * WebServer
 **/

void initServerRoutes() {
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html",false);
  });

  server.serveStatic("/", SPIFFS, "/");
}

/** 
 * WebSockets
 **/

String getOutputStates(){
//   JSONVar myArray;
//   for (int i =0; i<NUM_OUTPUTS; i++){
//     myArray["gpios"][i]["output"] = String(outputGPIOs[i]);
//     myArray["gpios"][i]["state"] = String(digitalRead(outputGPIOs[i]));
//   }
//   String jsonString = JSON.stringify(myArray);
  return "";//jsonString;
}

void notifyClients(String state) {
  ws.textAll(state);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "states") == 0) {
      notifyClients(getOutputStates());
    }
    else{
      int gpio = atoi((char*)data);
      digitalWrite(gpio, !digitalRead(gpio));
      notifyClients(getOutputStates());
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

/**
 * SETUP
 */
void setup(){
  Serial.begin(115200);

  Serial.println("** Filesystem **");
  initSPIFFS();

  Serial.println("** Network **");
  // WifiAP();  // Access Point
  WifiSTA(); // Wifi Client

  Serial.println("** Webservices **");
  // initWebSocket();
  initServerRoutes();

  Serial.println("** OTA Updater **");
  AsyncElegantOTA.begin(&server);
  
  // Start server
  server.begin();
}
/**
 * MAIN LOOP
 */
void loop() {
  AsyncElegantOTA.loop();
  // ws.cleanupClients();
}

