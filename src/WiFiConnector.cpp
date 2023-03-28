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

WiFiConnector::WiFiConnector(LedStripsManager* ledStripsManager) {
  this->ledStripsManager = ledStripsManager;
  this->ssid = EEPROMManager::readString(SSID_ADDRESS, 32);
  this->password = EEPROMManager::readString(PASS_ADDRESS, 64);

  this->_status = INITIALIZING;
  this->_apModeStarted = false;

  persWM = new PersWiFiManager();
  persWM->setConnectNonBlock(true);
  persWM->onAttemptConnection([&]() {
    Serial.println("Connecting...");
    _status = CONNECTING;
    });
  persWM->onConnect([&]() {
    Serial.println("wifi connected");
    Serial.println(WiFi.localIP());
    if (_apModeStarted) {
      persWM->stopServers();
      EEPROMManager::writeString(SSID_ADDRESS, WiFi.SSID());
      EEPROMManager::writeString(PASS_ADDRESS, WiFi.psk());
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
    EEPROMManager::clear(SSID_ADDRESS, PASS_ADDRESS - 1);
    EEPROMManager::clear(PASS_ADDRESS, EEPROM_SIZE - 1);
    });
}