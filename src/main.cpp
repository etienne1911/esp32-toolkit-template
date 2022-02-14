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

#include "../Dont-Commit-Me.h" // PUT Wifi configuration here
#include <iostream>
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <AsyncElegantOTA.h>
#include <WebSerial.h>
// #include <logging.hpp>
// #include <fs-appender.hpp>
// #include <ets-appender.hpp>
// #include <udp-appender.hpp>
#include <WebSocket.h>
#include <fs-utils.h>
#include <server-routes.h>
#include <web-controls.h>
#include <display.h>
#include <sensors.h>

const char *hotspotSSID = "******";
const char *hotspotPWD = "******";

const char *wifiSSID = customSSID;
const char *wifiPWD = customPWD;

// using namespace std;
// using namespace esp32m;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

int LED_PIN = 13;
int count = 0;

/**
 *  Loggin
 */
// void initLoggin(){
//   // SPIFFS.begin(true);
//     // send log messages to standard output (Serial)
//   Logging::addAppender(&ETSAppender::instance());
//   // send log messages to file "mylog" on the SPIFFS
//   Logging::addAppender(new FSAppender(LittleFS, "mylog"));
//   // send log messages to 192.168.1.1:1234 in the form of UDP packets
//   // Logging::addAppender(new UDPAppender("192.168.1.1", 1234));
//   // redirect standard output to appenders
//   Logging::hookUartLogger();
//   // now use log_X macros to forward log messages to the registered appenders
//   log_i("hello world!");
// }

/** 
 * FileSystem 
 **/
void initFileSys()
{
  if (!LittleFS.begin(true))
  {
    Serial.println("error while mounting filesystem");
  }
  Serial.println("mounted successfully!");

  File file = LittleFS.open("/test.txt", "r");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("File Content:");
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();

  listDir(LittleFS, "/", 0);
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
  // std::cout << "Connecting to " << wifiSSID << " network";
  Serial.print("Connecting to ");
  Serial.print(wifiSSID);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  std::cout << std::endl
            << "Connected with IP address: "; //<< WiFi.localIP() << std::endl;
  Serial.print("IP address:");
  Serial.println(WiFi.localIP());
}

/** 
 * WebSockets
 **/

void initWebSocket()
{
  WebSocketServerImpl::ws = &ws;
  // bind websocket event handler
  ws.onEvent(WebSocketServerImpl::eventHandler);
  // bind websocket to webserver
  server.addHandler(&ws);
}

/**
 * SETUP
 */
void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("");
  Serial.println("** Filesystem **");
  initFileSys();
  Serial.println("");

  // Serial.println("** Loggin **");
  // initLoggin();
  // Serial.println("switch logs to registered appenders");

  Serial.println("** Network **");
  // WifiAP();  // Access Point
  WifiSTA(); // Wifi Client
  Serial.println("");

  Serial.println("** Webservices **");
  initWebSocket();
  initServerRoutes(&server);
  Serial.println("OK");
  Serial.println("");

  Serial.println("** OTA Updater **");
  AsyncElegantOTA.begin(&server);
  Serial.println("OK");
  Serial.println("");

  Serial.println("** Web Serial Monitor & Controls **");
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  // WebSerial.msgCallback(recvMsg);
  Serial.println("OK");
  Serial.println("");

  // Start server
  server.begin();

  // Serial.println("** OLED Screen **");
  // Display display(14, 15, 0x3C);
  // display.testDisplay();
  resetPot();
  ina219Setup(14, 15);
  pinMode(LED_PIN, OUTPUT); // Set GPIO22 as digital output pin
  WebSerial.println("Done Setup!");
}

void blinkLED()
{
  digitalWrite(LED_PIN, HIGH); // Set GPIO22 active high
  // WebSerial.println("LED ON");
  delay(1000);
  ina219DisplayValues();
  digitalWrite(LED_PIN, LOW); // Set GPIO22 active low
  // WebSerial.println("LED OFF");
  delay(1000); // delay of one second
  ina219DisplayValues();
}
/**
 * MAIN LOOP
 */
void loop()
{
  AsyncElegantOTA.loop();
  ws.cleanupClients();
  // blinkLED();
  // digiPotLoop();
  count = count < 200000 ? count + 1 : 0;
  // update call
  if (count == 0)
  {
    cycle();
    ina219DisplayValues();
  }
}
