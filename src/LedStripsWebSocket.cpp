#include <LedStripsWebSocket.h>

void LedStripsWebSocket::begin() {
    if (!this->enabled())
        this->enable(true);
}

void LedStripsWebSocket::stop() {
    if (!this->enabled()) {
        this->enable(false);
        this->closeAll();
    }
}

void LedStripsWebSocket::loop() {
    this->cleanupClients();
}

void LedStripsWebSocket::onWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch (type) {
    case WS_EVT_CONNECT:
        Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
        break;
    case WS_EVT_ERROR:
        Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
        break;
    case WS_EVT_DATA: {
        AwsFrameInfo* info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len) {
            //the whole message is in a single frame and we got all of it's data
            Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);
            if (info->opcode == WS_TEXT) {
                String response = handleReceivedData(data);
                client->text(response);
            }
        }
    }
        break;
    default:
        break;
    }

}

String LedStripsWebSocket::handleReceivedData(uint8_t* data) {
    DynamicJsonDocument message(256);
    deserializeJson(message, data);
    String eventType = message["eventType"];
    String response;
    if (eventType.equals(INITIALIZE)) {
        return getLastStatus();
    } else if (eventType.equals(CHANGE_COLOR)) {
        JsonObject colorChangeData = message["payload"];
        uint8_t order = colorChangeData["order"], r = colorChangeData["r"], g = colorChangeData["g"], b = colorChangeData["b"];
        if (ledStripsManager->changeColor(order, r, g, b)) {
            serializeJson(message, response);
        } else {
            return createInvalidResponse("Couldn't change color for " + order);
        }
    } else if (eventType.equals(SWITCH_ON_OFF)) {
        bool state = message["payload"];
        state ? ledStripsManager->enable() : ledStripsManager->disable();
        serializeJson(message, response);
    } else {
        return createInvalidResponse("Unknown event type: " + eventType);
    }
    return response;
}

String LedStripsWebSocket::getLastStatus() {
    DynamicJsonDocument json(512);
    JsonObject root = json.to<JsonObject>();
    root["eventType"] = INITIALIZE;
    JsonObject payload = root.createNestedObject("payload");
    payload["enabled"] = ledStripsManager->isEnabled();
    const JsonArray& array = payload.createNestedArray("ledStrips");
    std::array<uint8_t, LED_STRIPS_NUMBER> orders = ledStripsManager->getLedStripsOrders();
    for (int i = 0; i < orders.size(); i++) {
        auto colors = ledStripsManager->getSavedColorsForLedStripByOrder(orders[i]);
        const JsonObject& ledStripObj = array.createNestedObject();
        ledStripObj["order"] = orders[i];
        ledStripObj["r"] = colors->getRed();
        ledStripObj["g"] = colors->getGreen();
        ledStripObj["b"] = colors->getBlue();
    }
    String response;
    serializeJson(json, response);
    return response;
}

String LedStripsWebSocket::createInvalidResponse(String errorMessage) {
    String response;
    DynamicJsonDocument invalid(64);
    invalid["eventType"] = INVALID;
    invalid["errorMessage"] = errorMessage;
    serializeJson(invalid, response);
    return response;
}

LedStripsWebSocket::LedStripsWebSocket(String url, LedStripsManager* ledStripsManager): AsyncWebSocket(url) {
    this->ledStripsManager = ledStripsManager;
    const auto event = std::bind(&LedStripsWebSocket::onWsEvent, this,
        std::placeholders::_1, std::placeholders::_2,
        std::placeholders::_3, std::placeholders::_4,
        std::placeholders::_5, std::placeholders::_6);
    this->onEvent(event);
}