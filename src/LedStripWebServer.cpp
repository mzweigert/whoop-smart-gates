#include <LedStripWebServer.h>


void LedStripWebServer::initEndpoints() {
  server->on("/", [] (AsyncWebServerRequest *request) {
    Serial.println("Send index...");
    request->send(LittleFS, "/index.html");
  });

  server->on("/getLedStripIds", [&] (AsyncWebServerRequest *request) {
    String response = "{ \"ids\" : [ ";
    std::array<String, LED_STRIPS_NUMBER> ids = ledStripsManager->getLedStripsIds();
    for(int i = 0; i < ids.size(); i++) {
      response += "\"" + ids[i] + "\"";
      if(i + 1 < ids.size()) {
        response += ", ";
      }
    }
    response += " ]}";
    request->send(200, "application/json", response);
  });

  server->on("/getLastStatus", [&] (AsyncWebServerRequest *request) {
    String response = "{ \"enabled\" : ";
    response += ledStripsManager->isEnabled();
    response += ", \"ledStrips\": [";
    std::array<String, LED_STRIPS_NUMBER> ids = ledStripsManager->getLedStripsIds();
    for(int i = 0; i < ids.size(); i++) {
        auto colors = ledStripsManager->getSavedColorsForLedStripById(ids[i]);
        response += "{";
        response += " \"id\" : \""; response += ids[i]; response += "\", ";
        response += "  \"r\" : "; response += colors->getRed(); response += ", ";
        response += "  \"g\" : "; response += colors->getGreen(); response += ", ";
        response += "  \"b\" : "; response += colors->getBlue();
        response += "}";
        if(i + 1 < ids.size()) {
            response += ", ";
        }
    }
    response += " ]}";
    request->send(200, "application/json", response);
  });

  server->on("/disable", HTTP_POST, [&] (AsyncWebServerRequest *request) {
      ledStripsManager->disable();
      request->send(200, "text/plain", "Disabled!");
  });

  server->on("/enable", HTTP_POST, [&] (AsyncWebServerRequest *request) {
      ledStripsManager->enable();
      request->send(200, "text/plain", "Enabled!");
  });


  AsyncCallbackJsonWebHandler* changeColorHandler = new AsyncCallbackJsonWebHandler("/changeColor", [&](AsyncWebServerRequest *request, JsonVariant &json) {
    StaticJsonDocument<200> data = json.as<JsonObject>();
    String id = data["id"];
    uint8_t red = data["r"], green = data["g"], blue = data["b"];

    if(ledStripsManager->changeColor(id, red, green, blue)) {
        request->send(200, "text/plain", "Changed!");
    } else {
        request->send(404, "text/plain", "404: Not Found");
    }
  });
  server->addHandler(changeColorHandler);

  server->on("/reset", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->redirect("/");
    DeviceReset::reset();
  });
}

LedStripWebServer::LedStripWebServer(LedStripsManager* ledStripsManager) {
    this->ledStripsManager = ledStripsManager;
    // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", " GET, POST, OPTIONS, PUT, DELETE");
    // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
    this->server = new AsyncWebServer(80);
    this->ledStripsManager->initColors();
    initEndpoints();
    this->server->serveStatic("/", LittleFS, "/");
    _isRunning = false;
}

void LedStripWebServer::begin() {
    LittleFS.begin();
    server->begin();
    _isRunning = true;
}

void LedStripWebServer::stop() {
  LittleFS.end();
  server->end();
  _isRunning = false;
}

bool LedStripWebServer::isRunning() {
  return _isRunning;
}