
#include <ArduinoJson.h>
#include <AsyncJson.h>

#include <LittleFS.h>
#include <ESPAsyncWebServer.h>
#include <DeviceReset.h>
#include <LedStripsManager.h>

class LedStripWebServer {
private:
    AsyncWebServer* server;
    LedStripsManager* ledStripsManager;
    bool _isRunning;
    void initEndpoints();

public:
    void begin();
    void stop();

    bool isRunning();

    LedStripWebServer(LedStripsManager* ledStripsManager);
};
