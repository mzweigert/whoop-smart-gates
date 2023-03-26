#ifndef WiFiConnector_h
#define WiFiConnector_h

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <FS.h>

#include <PersWiFiManager.h>
#include <EEPROMManager.h>
#include <DeviceReset.h>

#define DEBUG_PRINT(x) Serial.println(x)
#define DEVICE_NAME "ESP8266 DEVICE"

typedef enum {
    INITIALIZING = 0,
    IN_AP_MODE = 1,
    CONNECTING = 2,
    CONNECTED = 3
} conn_status;

class WiFiConnector {
 private:
  PersWiFiManager *persWM;
  String ssid, password;
  conn_status _status;
  bool _apModeStarted; // needed to save credentials and reset device due to servers overlaping
  
 public:
  conn_status status();
  bool connectToWiFi();
  void loop();
  void stop();

  bool canStopAPMode();

  WiFiConnector();

};

#endif
