#include <WiFiConnector.h>

conn_status WiFiConnector::status() {
  return _status;
}

void WiFiConnector::loop() {
  if (_status == INITIALIZING) {
    persWM->attemptConnection(ssid, password);
    ledStripsManager->blink(BLUE);
  } else if (_status == CONNECTING) {
    persWM->handleWiFi();
    ledStripsManager->blink(GREEN);
  } else if (_status == IN_AP_MODE) {
    if (!persWM->isRunning()) {
      persWM->begin();
    }
    persWM->loopServers();
    ledStripsManager->blink(RED);
  } else {
    _status = INITIALIZING;
  }
}

void WiFiConnector::initCredentials() {
  LittleFS.begin();
  File configFile = LittleFS.open(CONFIG_FILE_PATH, "r");
  if (!configFile) {
    Serial.println("Config file init null..");
    return;
  }

  this->ssid = configFile.readStringUntil(LINE_TERMINATOR);
  this->password = configFile.readStringUntil(LINE_TERMINATOR);
  configFile.close();
  LittleFS.end();
}

void WiFiConnector::saveCredentials() {
  LittleFS.begin();
  File configFile = LittleFS.open(CONFIG_FILE_PATH, "w+");
  if (!configFile) {
    Serial.println("Config file init null.. creating.");
    return;
  }

  configFile.print(WiFi.SSID());
  configFile.print(LINE_TERMINATOR);
  configFile.print(WiFi.psk());
  configFile.print(LINE_TERMINATOR);

  configFile.close();
  LittleFS.end();
}

WiFiConnector::WiFiConnector(LedStripsManager* ledStripsManager) {
  this->ledStripsManager = ledStripsManager;
  this->_status = INITIALIZING;
  this->_apModeStarted = false;
  this->initCredentials();

  persWM = new PersWiFiManager();
  persWM->setConnectNonBlock(true);
  persWM->onAttemptConnection([&]() {
    Serial.println("Connecting...");
    _status = CONNECTING;
  });
  persWM->onConnect([&]() {
    Serial.println("wifi connected");
    if (_apModeStarted) {
      persWM->stopServers();
      saveCredentials();
      Serial.println("Restarting...");
      DeviceReset::reset();
    }
    _status = CONNECTED;
  });

  persWM->onAp([&]() {
    Serial.println("AP MODE Initialized!");
    Serial.println(persWM->getApSsid());
    _status = IN_AP_MODE;
    _apModeStarted = true;
  });
}