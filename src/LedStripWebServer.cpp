#include <LedStripWebServer.h>

void LedStripWebServer::initEndpoints() {

  server->on("/getLedStripOrders", [&](AsyncWebServerRequest* request) {
    AsyncJsonResponse* response = new AsyncJsonResponse();
    const JsonArray& array = response->getRoot()
      .createNestedArray("orders");
    std::array<uint8_t, LED_STRIPS_NUMBER> orders = ledStripsManager->getLedStripsOrders();
    for (int i = 0; i < orders.size(); i++) {
      array.add(orders[i]);
    }
    response->setLength();
    request->send(response);
  });

  server->on("/getLastStatus", [&](AsyncWebServerRequest* request) {
    AsyncJsonResponse* response = new AsyncJsonResponse();
    const JsonObject& root = response->getRoot();
    root["enabled"] = ledStripsManager->isEnabled();
    const JsonArray& array = root.createNestedArray("ledStrips");
    std::array<uint8_t, LED_STRIPS_NUMBER> orders = ledStripsManager->getLedStripsOrders();
    for (int i = 0; i < orders.size(); i++) {
      auto colors = ledStripsManager->getSavedColorsForLedStripByOrder(orders[i]);
      const JsonObject& ledStripObj = array.createNestedObject();
      ledStripObj["order"] = orders[i];
      ledStripObj["r"] = colors->getRed();
      ledStripObj["g"] = colors->getGreen();
      ledStripObj["b"] = colors->getBlue();
    }
    response->setLength();
    request->send(response);
  });

  server->on("/disable", HTTP_POST, [&](AsyncWebServerRequest* request) {
    ledStripsManager->disable();
    request->send(200, "text/plain", "Disabled!");
  });

  server->on("/enable", HTTP_POST, [&](AsyncWebServerRequest* request) {
    ledStripsManager->enable();
    request->send(200, "text/plain", "Enabled!");
  });


  AsyncCallbackJsonWebHandler* changeColorHandler = new AsyncCallbackJsonWebHandler("/changeColor", [&](AsyncWebServerRequest* request, JsonVariant& json) {
    StaticJsonDocument<200> data = json.as<JsonObject>();
    uint8_t order = data["order"], red = data["r"], green = data["g"], blue = data["b"];

    if (ledStripsManager->changeColor(order, red, green, blue)) {
      request->send(200, "text/plain", "Changed!");
    } else {
      request->send(404, "text/plain", "404: Not Found");
    }
  });
  server->addHandler(changeColorHandler);

  server->on("/disconnect", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->redirect("/");
    WiFi.disconnect();
  });

  server->on("/reset", HTTP_GET, [](AsyncWebServerRequest* request) {
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

  initEndpoints();
  this->server->serveStatic("/", LittleFS, "/")
                .setDefaultFile("index.html");;
  _isRunning = false;
}

void LedStripWebServer::begin() {
  LittleFS.begin();
  server->begin();
  ledStripsManager->initColors();
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