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

typedef enum {
    INITIALIZING = 0,
    IN_AP_MODE = 1,
    CONNECTED = 2,
    STOPPED = 3
} conn_status;

class WiFiConnector {
 private:
  PersWiFiManager *persWM;

 public:
  conn_status status();
  bool connectToWiFi();
  void loop();
  void stop();

  bool canStopAPMode();

  WiFiConnector();

};

#endif
