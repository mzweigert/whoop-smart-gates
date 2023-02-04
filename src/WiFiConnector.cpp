#include "WiFiConnector.h"

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#include "PersWiFiManager.h"
#include "ESP8266WebServer.h"

#define DEBUG_PRINT(x) Serial.println(x)
#define DEVICE_NAME "ESP8266 DEVICE"

ESP8266WebServer server(80);
DNSServer dnsServer;
PersWiFiManager persWM(server, dnsServer);
bool wiFiConnected = false;
bool canStopServers = false;

bool WiFiConnector::isConnected() {
  return wiFiConnected;
}

bool WiFiConnector::connectToWiFi() {
  persWM.onConnect([]() {
    DEBUG_PRINT("wifi connected");
    DEBUG_PRINT(WiFi.localIP());
    wiFiConnected = canStopServers = true;
  });
  //...or AP mode is started
  persWM.onAp([]() {
    DEBUG_PRINT("AP MODE");
    DEBUG_PRINT(persWM.getApSsid());
  });

  LittleFS.begin();
  //sets network name for AP mode
  persWM.setApCredentials(DEVICE_NAME);

  //make connecting/disconnecting non-blocking
  persWM.setConnectNonBlock(true);

  server.begin();
  return persWM.begin();
}

void WiFiConnector::loop() {
  //in non-blocking mode, handleWiFi must be called in the main loop
  if (!wiFiConnected) {
    persWM.handleWiFi();
    dnsServer.processNextRequest();
    server.handleClient();
  } else if (canStopServers) {
    dnsServer.stop();
    server.stop();
    canStopServers = false;
  }
}
