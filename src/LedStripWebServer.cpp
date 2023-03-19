#include <LedStripWebServer.h>

#define SIZEOF(arr) sizeof(arr) / sizeof(*arr)
 
std::array<std::array<uint8_t, 3>, 3> ledStripsPins {
    {{FIRST_RED, FIRST_GREEN, FIRST_BLUE},  // FIRST
    {SECOND_RED, SECOND_GREEN, SECOND_BLUE},  // SECOND
    {THIRD_RED, THIRD_GREEN, THIRD_BLUE}}   // THIRD
};

std::map<uint8_t, LedStrip*> LedStripWebServer::initLedStrips() {
    std::map<uint8_t, LedStrip*> leds = std::map<uint8_t, LedStrip*>();
    for (int i = 0; i < ledStripsPins.size(); i++) {
        leds[i] = new LedStrip(ledStripsPins[i][0], ledStripsPins[i][1], ledStripsPins[i][2]);
        uint8_t enabled = EEPROMManager::readByte(ENABLED_ADDRESS);
        if (enabled) {
            uint8_t bytes[3];
            uint8_t *colors = EEPROMManager::readBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (i * 3), 3, bytes);
            leds[i]->rgb(*(colors), *(colors + 1), *(colors + 2));
        }
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

   server->on("/getLedStripIds", [&]() {
    String response = "{ \"ids\" : [ ";
    uint8_t added = 0;
    for(auto const& imap: ledStrips) {
        response += imap.first;
        added++;
        if(added < ledStrips.size()) {
            response += ", ";
        }
    }
    response += " ]}";
    server->send(200, "application/json", response);
  });

  
  server->on("/getLastStatus", [&]() {
    String response = "{ \"enabled\" : ";
    response += EEPROMManager::readByte(ENABLED_ADDRESS) > 0;
    response += ", \"ledStrips\": [";
    uint8_t added = 0;
    for(auto const& imap: ledStrips) {
        uint8_t bytes[3];
        uint8_t* colors = EEPROMManager::readBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (imap.first * 3), 3, bytes);
        response += "{";
        response += " \"id\" : "; response +=imap.first; response += ", ";
        response += "  \"r\" : "; response += *(colors); response += ", ";
        response += "  \"g\" : "; response += *(colors + 1); response += ", ";
        response += "  \"b\" : "; response += *(colors + 2);
        response += "}";
        added++;
        if(added < ledStrips.size()) {
            response += ", ";
        }
    }
    response += " ]}";
    server->send(200, "application/json", response);
  });

  server->on("/disable", HTTP_POST, [&]() {
      for(auto const& imap: ledStrips) {
        imap.second->disable();
      }
      EEPROMManager::writeByte(ENABLED_ADDRESS, OFF);
      server->send(200, "text/plain", "Disabled!");
  });

  server->on("/enable", HTTP_POST, [&]() {
      for(auto const& imap: ledStrips) {
        uint8_t bytes[3];
        uint8_t* colors = EEPROMManager::readBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (imap.first * 3), 3, bytes);
        imap.second->rgb(*(colors), *(colors + 1), *(colors + 2));
      }
      EEPROMManager::writeByte(ENABLED_ADDRESS, ON);
      server->send(200, "text/plain", "Enabled!");
  });

  server->on("/changeColor", HTTP_POST, [&]() {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, server->arg("plain"));
    uint8_t id = doc["id"], red = doc["r"], green = doc["g"], blue = doc["b"];
    if(ledStrips.find(id) != ledStrips.end()) {
        uint8_t colors[] = {red, green, blue};
        EEPROMManager::writeBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (id * 3), colors, SIZEOF(colors));
        uint8_t enabled = EEPROMManager::readByte(ENABLED_ADDRESS);
        if (enabled) {
            LedStrip* ledStrip = ledStrips[id];
            ledStrip->rgb(red, green, blue);
        }
        server->send(200, "text/plain", "Changed!");
    } else {
        server->send(404, "text/plain", "404: Not Found");
    }
  });

  server->on("/reset", HTTP_GET, [&]() {
    digitalWrite(16, LOW);
    server->send(200, "text/plain", "Reset!");
  });
}

LedStripWebServer::LedStripWebServer() {
    ledStrips = initLedStrips();
    _isRunning = false;
}

void LedStripWebServer::begin() {
    LittleFS.begin();
    server = new ESP8266WebServer(80);
    initEndpoints();
    server->enableCORS(true);
    server->serveStatic("/", LittleFS, "/");
    server->begin();
    _isRunning = true;
}

void LedStripWebServer::stop() {
  server->stop();
  _isRunning = false;
}
void LedStripWebServer::loop() {
  server->handleClient();
}

bool LedStripWebServer::isRunning() {
  return _isRunning;
}