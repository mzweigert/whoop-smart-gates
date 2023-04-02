#ifndef PERSWIFIMANAGER_H
#define PERSWIFIMANAGER_H

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#define AP_MODE_IP IPAddress(192, 168, 1, 1)
#define SUBNET IPAddress(255, 255, 255, 0)

#define WIFI_CONNECT_TIMEOUT 30

#define CONNECTION_RETRIES 3

class PersWiFiManager {

 public:
  typedef std::function<void(void)> WiFiChangeHandlerFunction;

  PersWiFiManager();

  bool attemptConnection(const String& ssid, const String& pass, const String& ip, const String& gateway);

  void setConnectNonBlock(bool b);

  void handleWiFi();

  void onConnect(WiFiChangeHandlerFunction fn);

  void onAttemptConnection(WiFiChangeHandlerFunction fn);

  void onAp(WiFiChangeHandlerFunction fn);

  void begin();

  void loopServers();

  void stopServers();

  bool isRunning();

  String getApSsid();

 private:
  void setupWiFiHandlers();
  void startApMode();
  void setApCredentials(const String& apSsid, const String& apPass = "");
  bool isConnectionTimeoutReached();
  bool startConnection(const String& ssid, const String& pass, const String& localIp, const String& gateway);

  AsyncWebServer* _server;
  DNSServer* _dnsServer;
  String _apSsid, _apPass;
  bool _isRunning;

  bool _connectNonBlock;
  unsigned long _connectStartTime;
  uint8_t _retries;
  bool _freshConnectionAttempt;

  WiFiChangeHandlerFunction _connectHandler;
  WiFiChangeHandlerFunction _attemptConnectionHandler;
  WiFiChangeHandlerFunction _apHandler;

};  // class

#endif
