#ifndef WiFiConnector_h
#define WiFiConnector_h

#include <DNSServer.h>
#include <DeviceReset.h>
#include <EEPROMManager.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <LedStripsManager.h>
#include <PersWiFiManager.h>

#define LINE_TERMINATOR '\n'
#define CONFIG_FILE_PATH "/config/config.txt"

typedef enum {
  INITIALIZING = 0,
  IN_AP_MODE = 1,
  CONNECTING = 2,
  CONNECTED = 3
} conn_status;

class WiFiConnector {
 private:
  PersWiFiManager* persWM;
  LedStripsManager* ledStripsManager;
  String ssid, password, localIp, gatewayIp;
  conn_status _status;
  unsigned long _startTimeApMode;
  bool _apModeStarted;  // needed to save credentials and reset device due to servers overlaping

  void initCredentials();
  void saveCredentials();

 public:
  conn_status status();
  bool connectToWiFi();
  void loop();
  void stop();

  bool canStopAPMode();

  WiFiConnector(LedStripsManager* ledStripsManager);
};

#endif
