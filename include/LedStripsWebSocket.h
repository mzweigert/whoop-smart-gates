#ifndef LedStripsWebSocket_h
#define LedStripsWebSocket_h

#include <ArduinoJson.h>
#include <AsyncJson.h>

#include <LedStripsManager.h>

#define LED_STRIPS_WS_URL "/led-strips-ws"

#define INITIALIZE "INITIALIZE"
#define CHANGE_COLOR "CHANGE_COLOR"
#define SWITCH_ON_OFF "SWITCH_ON_OFF"
#define INVALID "INVALID"

class LedStripsWebSocket: protected AsyncWebSocket {
private:
    LedStripsManager* ledStripsManager;
    bool _isRunning;

    void onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t length);

    String handleReceivedData(uint8_t* data);
    String getLastStatus();
    String createInvalidResponse(String errorMessage);

public:
    void begin();
    void stop();
    void loop();

    LedStripsWebSocket(String url, LedStripsManager* ledStripsManager);
};
#endif