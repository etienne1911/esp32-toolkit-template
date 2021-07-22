#include <iostream>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

/**
 * Websocket server static implementation example
 * A client sends its data to the server.
 * The server notify all connected clients to synchronize them
 */
class WebSocketServerImpl
{
public:
    // reference to the websocket object
    static AsyncWebSocket *ws;

    // WebSocketServerImplem(AsyncWebSocket *wsRef)
    // {
    //     ws = wsRef;
    // }

    // websocket event handler
    static void eventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                             void *arg, uint8_t *data, size_t len);

    static void processClientMessage(void *arg, uint8_t *data, size_t len);

    static void parseInput(String dataIn);

    // Server response to clients
    // @return: a jsonified string
    static String serverOutput();
};

AsyncWebSocket* WebSocketServerImpl::ws =  null;

void WebSocketServerImpl::eventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                                       void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        processClientMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void WebSocketServerImpl::processClientMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        data[len] = 0;
        // convert raw data input
        std::string dataIn(data, data + len);
        // do some process
        String dataOut(dataIn.c_str());
        // notify server response to all clients
        ws->textAll(dataOut);
    }
}

String WebSocketServerImpl::serverOutput()
{
    // std::string jsonString = "a jsonified string";
    JSONVar jsarr;
    jsarr["root"]["child"] = String("childValue");
    String jsonStringOut = JSON.stringify(jsarr);
    return jsonStringOut; //sJsonOutput;
};
