#include <WiFiConnector.h>

conn_status WiFiConnector::status() {
  return _status;
}

void WiFiConnector::stop() {
  persWM->stopServers();
}

bool WiFiConnector::canStopAPMode() {
  return persWM->isRunning();
}

void WiFiConnector::loop() {
    if(_status == INITIALIZING || !persWM->isRunning()) {
        persWM->begin();
        persWM->attemptConnection(ssid, password);
        _status = CONNECTING;
    } 
    
    persWM->handleWiFi();
    persWM->loopServers();
}

WiFiConnector::WiFiConnector() {
  this->persWM = new PersWiFiManager();
  this->ssid =  EEPROMManager::readString(SSID_ADDRESS, 32);
  this->password = EEPROMManager::readString(PASS_ADDRESS, 64);

  persWM->setConnectNonBlock(true);
  persWM->onConnect([&]() {
    DEBUG_PRINT("wifi connected");
    DEBUG_PRINT(WiFi.localIP());
    _status = CONNECTED;
    EEPROMManager::writeString(SSID_ADDRESS, WiFi.SSID());
    EEPROMManager::writeString(PASS_ADDRESS, WiFi.psk());
  });
  //...or AP mode is started
  persWM->onAp([&]() {
    DEBUG_PRINT("AP MODE Initialized!");
    DEBUG_PRINT(DEVICE_NAME);
    _status = IN_AP_MODE;
    EEPROMManager::clear(SSID_ADDRESS, PASS_ADDRESS - 1);
    EEPROMManager::clear(PASS_ADDRESS, EEPROM_SIZE - 1);
  });
  _status = INITIALIZING;
}