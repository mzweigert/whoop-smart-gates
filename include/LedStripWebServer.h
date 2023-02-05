#include <map>
#include <inttypes.h>
#include <ArduinoJson.h>

#include <LittleFS.h>
#include <RGBPins.h>
#include <LedStrip.h>
#include <ESP8266WebServer.h>


class LedStripWebServer {
private:
    std::map<uint8_t, LedStrip*> ledStrips;
    ESP8266WebServer* server;

    uint8_t clampValue(uint8_t value);
    std::map<uint8_t, LedStrip*> initLedStrips();
    void initEndpoints();
public:
    void loop();
    LedStripWebServer();
};

