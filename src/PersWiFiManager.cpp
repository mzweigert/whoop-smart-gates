/* PersWiFiManager
   version 3.0.1
   https://r-downing.github.io/PersWiFiManager/
*/

#include "PersWiFiManager.h"

PersWiFiManager::PersWiFiManager() {
  _server = new ESP8266WebServer(80);
  _dnsServer = new DNSServer();
  _apPass = "";
}  //PersWiFiManager

bool PersWiFiManager::attemptConnection(const String& ssid, const String& pass) {
  //attempt to connect to wifi
  WiFi.mode(WIFI_STA);
  if (ssid.length()) {
    if (pass.length())
      WiFi.begin(ssid.c_str(), pass.c_str());
    else
      WiFi.begin(ssid.c_str());
  } else {
    WiFi.begin();
  }

  //if in nonblock mode, skip this loop
  _connectStartTime = millis();  // + 1;
  while (!_connectNonBlock && _connectStartTime) {
    handleWiFi();
    delay(10);
  }

  return WiFi.status() == WL_CONNECTED;
}  //attemptConnection

void PersWiFiManager::handleWiFi() {
  if (!_connectStartTime) return;

  if (WiFi.status() == WL_CONNECTED) {
    _connectStartTime = 0;
    if (_connectHandler) _connectHandler();
    return;
  }

  //if failed or not connected and time is up
  if ((WiFi.status() == WL_CONNECT_FAILED) ||
      ((WiFi.status() != WL_CONNECTED) &&
       ((millis() - _connectStartTime) > (1000 * WIFI_CONNECT_TIMEOUT)))) {
    startApMode();
    _connectStartTime = 0;  //reset connect start time
  }

}  //handleWiFi

void PersWiFiManager::startApMode() {
  //start AP mode
  IPAddress apIP(192, 168, 1, 1);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  _apPass.length() ? WiFi.softAP(getApSsid().c_str(), _apPass.c_str()) : WiFi.softAP(getApSsid().c_str());
  if (_apHandler) _apHandler();
}  //startApMode

void PersWiFiManager::setConnectNonBlock(bool b) {
  _connectNonBlock = b;
}  //setConnectNonBlock

void PersWiFiManager::setupWiFiHandlers() {
  IPAddress apIP(192, 168, 1, 1);
  _dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  _dnsServer->start((byte)53, "*", apIP);  //used for captive portal in AP mode

  _server->on("/wifi/list", [&]() {
    //scan for wifi networks
    int n = WiFi.scanNetworks();

    //build array of indices
    int ix[n];
    for (int i = 0; i < n; i++) ix[i] = i;

    //sort by signal strength
    for (int i = 0; i < n; i++)
      for (int j = 1; j < n - i; j++)
        if (WiFi.RSSI(ix[j]) > WiFi.RSSI(ix[j - 1])) std::swap(ix[j], ix[j - 1]);
    //remove duplicates
    for (int i = 0; i < n; i++)
      for (int j = i + 1; j < n; j++)
        if (WiFi.SSID(ix[i]).equals(WiFi.SSID(ix[j])) &&
            WiFi.encryptionType(ix[i]) == WiFi.encryptionType(ix[j])) ix[j] = -1;

    //build plain text string of wifi info
    //format [signal%]:[encrypted 0 or 1]:SSID
    String s = "";
    s.reserve(2050);
    for (int i = 0; i < n && s.length() < 2000; i++) {  //check s.length to limit memory usage
      if (ix[i] != -1) {
        s += String(i ? "\n" : "") + ((constrain(WiFi.RSSI(ix[i]), -100, -50) + 100) * 2) + "," +
             ((WiFi.encryptionType(ix[i]) == ENC_TYPE_NONE) ? 0 : 1) + "," + WiFi.SSID(ix[i]);
      }
    }

    //send string to client
    _server->send(200, "text/plain", s);
  });  //_server->on /wifi/list

  _server->on("/wifi/connect", HTTP_POST, [&]() {
    String ssid = _server->arg("n"), pass = _server->arg("p");
    attemptConnection(ssid, pass);
    _server->send(200, "text/html", "connecting...");
  });  //_server->on /wifi/connect

  _server->on("/wifi/ap", [&]() {
    _server->send(200, "text/html", "access point: " + getApSsid());
    startApMode();
  });  //_server->on /wifi/ap

  _server->on("/wifi/rst", [&]() {
    _server->send(200, "text/html", "Rebooting...");
    delay(100);
    //ESP.restart();
    // Adding Safer Restart method
    ESP.wdtDisable();
    ESP.reset();
    delay(2000);
  });

  _server->on("/", HTTP_GET, [&]() {
    File index = LittleFS.open("/wifi.html", "r");
    if (index) {
       _server->send(200, "text/html", index);
    } else {
      _server->send(404, "text/plain", "404: Not Found");
    };
  });
}  //setupWiFiHandlers

void PersWiFiManager::begin() {
  LittleFS.begin();
  _server->begin();
  setupWiFiHandlers();
}  //begin

void PersWiFiManager::loopServers(){
    _dnsServer->processNextRequest();
    _server->handleClient();
}

void PersWiFiManager::stopServers() {
   LittleFS.end();
   _dnsServer->stop();
  _server->stop();
}

String PersWiFiManager::getApSsid() {
  return _apSsid.length() ? _apSsid : "ESP8266";
}  //getApSsid

void PersWiFiManager::setApCredentials(const String& apSsid, const String& apPass) {
  if (apSsid.length()) _apSsid = apSsid;
  if (apPass.length() >= 8) _apPass = apPass;
}  //setApCredentials

void PersWiFiManager::onConnect(WiFiChangeHandlerFunction fn) {
  _connectHandler = fn;
}

void PersWiFiManager::onAp(WiFiChangeHandlerFunction fn) {
  _apHandler = fn;
}
