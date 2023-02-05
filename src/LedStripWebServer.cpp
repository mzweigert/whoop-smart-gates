#include <LedStripWebServer.h>

std::array<std::array<uint8_t, 3>, 3> ledStripsPins {
    {{FIRST_RED, FIRST_GREEN, FIRST_BLUE},  // FIRST
    {SECOND_RED, SECOND_GREEN, SECOND_BLUE},  // SECOND
    {THIRD_RED, THIRD_GREEN, THIRD_BLUE}}   // THIRD
};

std::map<uint8_t, LedStrip*> LedStripWebServer::initLedStrips() {
    std::map<uint8_t, LedStrip*> leds = std::map<uint8_t, LedStrip*>();
    for (int i = 0; i < ledStripsPins.size(); i++) {
        leds[i] = new LedStrip(ledStripsPins[i][0], ledStripsPins[i][1], ledStripsPins[i][2]);
    }
  return leds;
}

void LedStripWebServer::initEndpoints() {
  server->on("/", [&]() {
    File index = LittleFS.open("/index.html", "r");
    if (index) {
       server->send(200, "text/html", index);
    } else {
      server->send(404, "text/plain", "404: Not Found");
    };
  });

  server->on("/changeColor", HTTP_POST, [&]() {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, server->arg("plain"));
    uint8_t id = doc["id"], red = doc["red"], green = doc["green"], blue = doc["blue"];
    if(ledStrips.find(id) != ledStrips.end()) {
        LedStrip* ledStrip = ledStrips[id];
        ledStrip->red(red);
        ledStrip->green(green);
        ledStrip->blue(blue);
        server->send(200, "text/plain", "Changed!");
    } else {
        server->send(404, "text/plain", "404: Not Found");
    }
  });
}

LedStripWebServer::LedStripWebServer(/* args */) {
    ledStrips = initLedStrips();
    LittleFS.begin();
    server = new ESP8266WebServer(80);
    initEndpoints();
    server->begin();
}

void LedStripWebServer::loop() {
  server->handleClient();
}
