#ifndef WiFiConnector_h
#define WiFiConnector_h

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#include <PersWiFiManager.h>
#include <ESP8266WebServer.h>
#include <EEPROMManager.h>

#define DEBUG_PRINT(x) Serial.println(x)
#define DEVICE_NAME "ESP8266 DEVICE"

class WiFiConnector {
 private:
  PersWiFiManager *persWM;

 public:
  bool isConnected();
  bool connectToWiFi();
  void loop();

  WiFiConnector();
  ~WiFiConnector();

};

#endif
