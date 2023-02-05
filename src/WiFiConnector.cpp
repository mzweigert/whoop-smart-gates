#include <WiFiConnector.h>

bool wiFiConnected = false;

bool WiFiConnector::isConnected() {
  return wiFiConnected;
}

bool WiFiConnector::connectToWiFi() {
  persWM->onConnect([]() {
    DEBUG_PRINT("wifi connected");
    DEBUG_PRINT(WiFi.localIP());
    wiFiConnected = true;
    EEPROMManager::writeString(SSID_ADDRESS, WiFi.SSID());
    EEPROMManager::writeString(PASS_ADDRESS, WiFi.psk());
  });
  //...or AP mode is started
  persWM->onAp([]() {
    DEBUG_PRINT("AP MODE Initialized!");
    DEBUG_PRINT(DEVICE_NAME);
  });

  String ssid = EEPROMManager::readString(SSID_ADDRESS, 32);
  String password = EEPROMManager::readString(PASS_ADDRESS, 64);
  persWM->attemptConnection(ssid, password);

  uint8_t status = WiFi.waitForConnectResult();

  if(status != WL_CONNECTED) {
    DEBUG_PRINT("Intitialize AP MODE...");
    //sets network name for AP mode
    persWM->setApCredentials(DEVICE_NAME);
    //make connecting/disconnecting non-blocking
    persWM->setConnectNonBlock(true);
    persWM->begin();
    return false;
  } 
  DEBUG_PRINT("wifi connected");
  DEBUG_PRINT(WiFi.localIP());
  wiFiConnected = true;
  return true;
}

void WiFiConnector::loop() {
  //in non-blocking mode, handleWiFi must be called in the main loop
  if (!wiFiConnected) {
    persWM->handleWiFi();
    persWM->loopServers();
  } 
}
WiFiConnector::WiFiConnector() {
  this->persWM = new PersWiFiManager();
}

WiFiConnector::~WiFiConnector() {
  DEBUG_PRINT("Stop AP servers...");
  persWM->stopServers();
}
