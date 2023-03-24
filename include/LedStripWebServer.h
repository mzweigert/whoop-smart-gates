#include <map>
#include <inttypes.h>
#include <ArduinoJson.h>

#include <LittleFS.h>
#include <RGBPins.h>
#include <LedStrip.h>
#include <ESPAsyncWebServer.h>
#include <EEPROMManager.h>

class LedStripWebServer {
private:
    std::map<uint8_t, LedStrip*> ledStrips;
    AsyncWebServer* server;
    bool _isRunning;

    uint8_t clampValue(uint8_t value);
    std::map<uint8_t, LedStrip*> initLedStrips();
    void initEndpoints();
    void saveToEEPROM(uint8_t id);
public:
    void begin();
    void loop();
    void stop();

    bool isRunning();

    LedStripWebServer();
};
