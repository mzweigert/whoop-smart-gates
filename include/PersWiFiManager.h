#ifndef PERSWIFIMANAGER_H
#define PERSWIFIMANAGER_H

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>


#define WIFI_CONNECT_TIMEOUT 30

class PersWiFiManager {

 public:
  typedef std::function<void(void)> WiFiChangeHandlerFunction;

  PersWiFiManager();

  bool attemptConnection(const String& ssid = "", const String& pass = "");

  void setupWiFiHandlers();

  void begin();

  String getApSsid();

  void setApCredentials(const String& apSsid, const String& apPass = "");

  void setConnectNonBlock(bool b);

  void handleWiFi();

  void startApMode();

  void onConnect(WiFiChangeHandlerFunction fn);

  void onAp(WiFiChangeHandlerFunction fn);

  void loopServers();

  void stopServers();

  bool isRunning();

 private:
  ESP8266WebServer* _server;
  DNSServer* _dnsServer;
  String _apSsid, _apPass;
  bool _isRunning;
  
  bool _connectNonBlock;
  unsigned long _connectStartTime;
  bool _freshConnectionAttempt;

  WiFiChangeHandlerFunction _connectHandler;
  WiFiChangeHandlerFunction _apHandler;

};  //class

#endif
