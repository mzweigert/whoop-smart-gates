#ifndef LedStripsWebServer_h
#define LedStripsWebServer_h

#include <ArduinoJson.h>
#include <AsyncJson.h>

#include <LittleFS.h>
#include <ESPAsyncWebServer.h>
#include <DeviceReset.h>
#include <LedStripsManager.h>
#include <LedStripsWebSocket.h>

class LedStripsWebServer {
private:
    AsyncWebServer* server;
    LedStripsWebSocket* ws;
    LedStripsManager* ledStripsManager;
    bool _isRunning;
    void initEndpoints();

public:
    void begin();
    void stop();
    void loop();

    bool isRunning();

    LedStripsWebServer(LedStripsManager* ledStripsManager);
};
#endif