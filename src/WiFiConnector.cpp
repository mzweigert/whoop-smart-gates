#include <WiFiConnector.h>

conn_status WiFiConnector::status() {
  return _status;
}

void WiFiConnector::loop() {
    if(_status == INITIALIZING) {
        persWM->attemptConnection(ssid, password);
    } else if (_status == CONNECTING){
          persWM->handleWiFi();
    } else if (_status == IN_AP_MODE){
      if(!persWM->isRunning()) {
        persWM->begin();
      }
      persWM->loopServers();
    } else {
      _status = INITIALIZING;
    }
}

WiFiConnector::WiFiConnector() {
  ssid =  EEPROMManager::readString(SSID_ADDRESS, 32);
  password = EEPROMManager::readString(PASS_ADDRESS, 64);

  _status = INITIALIZING;
  _apModeStarted = false;

  persWM = new PersWiFiManager();
  persWM->setConnectNonBlock(true);
  persWM->onAttemptConnection([&]() {
    DEBUG_PRINT("Connecting...");
    _status = CONNECTING;
  });
  persWM->onConnect([&]() {
    DEBUG_PRINT("wifi connected");
    DEBUG_PRINT(WiFi.localIP());
    if(_apModeStarted) {
      persWM->stopServers();
      EEPROMManager::writeString(SSID_ADDRESS, WiFi.SSID());
      EEPROMManager::writeString(PASS_ADDRESS, WiFi.psk());
      DEBUG_PRINT("Restarting...");
      DeviceReset::reset();
    }
    _status = CONNECTED;
  });

  persWM->onAp([&]() {
    DEBUG_PRINT("AP MODE Initialized!");
    DEBUG_PRINT(DEVICE_NAME);
    _status = IN_AP_MODE;
    _apModeStarted = true;
    EEPROMManager::clear(SSID_ADDRESS, PASS_ADDRESS - 1);
    EEPROMManager::clear(PASS_ADDRESS, EEPROM_SIZE - 1);
  });
}