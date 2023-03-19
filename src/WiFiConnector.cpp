#include <WiFiConnector.h>

static conn_status _status;

conn_status WiFiConnector::status() {
  return _status;
}

bool WiFiConnector::connectToWiFi() {
  String ssid = EEPROMManager::readString(SSID_ADDRESS, 32);
  String password = EEPROMManager::readString(PASS_ADDRESS, 64);

  persWM->attemptConnection(ssid, password);

  uint8_t status = WiFi.waitForConnectResult();
  DEBUG_PRINT(ssid);
  DEBUG_PRINT(password);
  DEBUG_PRINT(status);
  if(status != WL_CONNECTED) {
    DEBUG_PRINT("Intitialize AP MODE...");
    //sets network name for AP mode
    persWM->setApCredentials(DEVICE_NAME);
    return false;
  } 
  return true;
}

void WiFiConnector::stop() {
  persWM->stopServers();
  _status = STOPPED;
}

bool WiFiConnector::canStopAPMode() {
  return persWM->isRunning();
}

void WiFiConnector::loop() {
    if(_status == INITIALIZING || !persWM->isRunning()) {
        persWM->begin();
        _status = connectToWiFi() ? CONNECTED : IN_AP_MODE;
    } 
    
    persWM->handleWiFi();
    persWM->loopServers();
}

WiFiConnector::WiFiConnector() {
  this->persWM = new PersWiFiManager();
  persWM->setConnectNonBlock(true);
  persWM->onConnect([]() {
    DEBUG_PRINT("wifi connected");
    DEBUG_PRINT(WiFi.localIP());
    _status = CONNECTED;
    EEPROMManager::writeString(SSID_ADDRESS, WiFi.SSID());
    EEPROMManager::writeString(PASS_ADDRESS, WiFi.psk());
  });
  //...or AP mode is started
  persWM->onAp([]() {
    DEBUG_PRINT("AP MODE Initialized!");
    DEBUG_PRINT(DEVICE_NAME);
    _status = IN_AP_MODE;
    EEPROMManager::clear(SSID_ADDRESS, PASS_ADDRESS - 1);
    EEPROMManager::clear(PASS_ADDRESS, EEPROM_SIZE - 1);
  });
  _status = INITIALIZING;
}