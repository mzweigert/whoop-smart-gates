#include <LedStripsWebServer.h>

void LedStripsWebServer::initEndpoints() {

  server->on("/disconnect", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->redirect("/");
    WiFi.disconnect();
  });

  server->on("/reset", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->redirect("/");
    DeviceReset::reset();
  });
}

LedStripsWebServer::LedStripsWebServer(LedStripsManager* ledStripsManager) {
  this->ledStripsManager = ledStripsManager;
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", " GET, POST, OPTIONS, PUT, DELETE");
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
  _isRunning = false;
}

void LedStripsWebServer::begin() {
  EEPROMManager::init();
  LittleFS.begin();
  this->server = new AsyncWebServer(80);
  this->ws = new LedStripsWebSocket(LED_STRIPS_WS_URL, ledStripsManager);
  this->server->addHandler((AsyncWebSocket*)this->ws);
  initEndpoints();
  this->server->serveStatic("/", LittleFS, "/sta")
      .setDefaultFile("index.html");
  server->begin();
  ws->begin();
  ledStripsManager->initColors();
  _startTimeServerMode = millis();
  _isRunning = true;
}

void LedStripsWebServer::loop() {
  if (_startTimeServerMode && DeviceReset::shouldReset &&
        millis() - _startTimeServerMode >= RESET_STA_MODE_TIME) {
      DeviceReset::reset();
  }
  this->ws->loop();
}

void LedStripsWebServer::stop() {
  LittleFS.end();
  ws->stop();
  server->end();
  ws = NULL;
  server = NULL;
  _isRunning = false;
}

bool LedStripsWebServer::isRunning() {
  return _isRunning;
}