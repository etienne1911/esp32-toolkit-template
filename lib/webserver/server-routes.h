

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

/** 
 * WebServer routes
 **/
void initServerRoutes(AsyncWebServer *server)
{
    // Route for root / web page
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/index.html", "text/html", false);
    });

    server->serveStatic("/", LittleFS, "/");
}