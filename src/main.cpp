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
#include <LITTLEFS.h>
#include <AsyncElegantOTA.h>
#include <WebSocket.h>

const char *hotspotSSID = "******";
const char *hotspotPWD = "******";

const char *wifiSSID = "Airbox7897";
const char *wifiPWD = "mdp26567832";

// using namespace std;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");

      Serial.print(file.name());
      time_t t = file.getLastWrite();
      struct tm *tmstruct = localtime(&t);
      Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);

      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");

      Serial.print(file.size());
      time_t t = file.getLastWrite();
      struct tm *tmstruct = localtime(&t);
      Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    }
    file = root.openNextFile();
  }
}

/** 
 * FileSystem 
 **/
void initFileSys()
{
  if (!LITTLEFS.begin(true))
  {
    Serial.println("error while mounting filesystem");
  }
  Serial.println("mounted successfully");

  // File file = LITTLEFS.open("/index.html", "r");
  // if (!file)
  // {
  //   Serial.println("Failed to open file for reading");
  //   return;
  // }

  // Serial.println("File Content:");
  // while (file.available())
  // {
  //   Serial.write(file.read());
  // }
  // file.close();
  listDir(LITTLEFS, "/", 0);
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
 * WebServer
 **/

void initServerRoutes()
{
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LITTLEFS, "/index.html", "text/html", false);
  });

  server.serveStatic("/", LITTLEFS, "/");
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

  Serial.println("** Network **");
  // WifiAP();  // Access Point
  WifiSTA(); // Wifi Client
  Serial.println("");

  Serial.println("** Webservices **");
  initWebSocket();
  initServerRoutes();
  Serial.println("OK");
  Serial.println("");

  Serial.println("** OTA Updater **");
  AsyncElegantOTA.begin(&server);
  Serial.println("OK");
  Serial.println("");

  // Start server
  server.begin();
}
/**
 * MAIN LOOP
 */
void loop()
{
  AsyncElegantOTA.loop();
  ws.cleanupClients();
}
