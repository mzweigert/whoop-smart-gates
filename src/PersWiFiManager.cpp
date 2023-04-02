/* PersWiFiManager
   version 3.0.1
   https://r-downing.github.io/PersWiFiManager/
*/

#include "PersWiFiManager.h"

PersWiFiManager::PersWiFiManager() {
  _server = new AsyncWebServer(80);
  _dnsServer = new DNSServer();
  _apPass = "";
  _freshConnectionAttempt = false;
  _isRunning = false;
}  // PersWiFiManager

bool PersWiFiManager::attemptConnection(const String& ssid, const String& pass, const String& ip, const String& gateway) {
  // attempt to connect to wifi
  if (_attemptConnectionHandler) _attemptConnectionHandler();

  bool connectionStarted = startConnection(ssid, pass, ip, gateway);
  if (!connectionStarted) {
    return connectionStarted;
  }

  // if in nonblock mode, skip this loop
  _connectStartTime = millis();  // + 1;
  _retries = 0;
  while (!_connectNonBlock && _connectStartTime) {
    handleWiFi();
    delay(10);
  }

  return (WiFi.status() == WL_CONNECTED);

}  // attemptConnection

void PersWiFiManager::handleWiFi() {
  if (!_connectStartTime) return;

  if (WiFi.status() == WL_CONNECTED) {
    _connectStartTime = _retries = 0;
    if (_connectHandler) _connectHandler();
  } else if (_freshConnectionAttempt || _retries >= CONNECTION_RETRIES) {
    startApMode();
    _connectStartTime = 0;  // reset connect start time
    _freshConnectionAttempt = false;
  } else if (isConnectionTimeoutReached() &&
             startConnection(WiFi.SSID(), WiFi.psk(), WiFi.localIP().toString(), WiFi.gatewayIP().toString())) {
    _connectStartTime = millis();
    Serial.println("Try " + String(_retries) + " connection failed!");
    _retries++;
  }
}  // handleWiFi

bool PersWiFiManager::startConnection(const String& ssid, const String& pass, const String& ip, const String& gateway) {
  WiFi.mode(WIFI_STA);
  IPAddress localIp, gatewayIp;
  if (localIp.fromString(ip) && gatewayIp.fromString(gateway)) {
    WiFi.config(localIp, gatewayIp, SUBNET);
  }
  if (ssid.length()) {
    WiFi.disconnect();  // To avoid issues (experience from WiFiManager)
    if (pass.length())
      WiFi.begin(ssid.c_str(), pass.c_str());
    else
      WiFi.begin(ssid.c_str());
  } else if ((WiFi.SSID() == "") && (WiFi.status() != WL_CONNECTED)) {
    // No saved credentials, so skip trying to connect
    _connectStartTime = millis();
    _freshConnectionAttempt = true;
    return false;
  } else {
    WiFi.begin();
  }
  return true;
}

bool PersWiFiManager::isConnectionTimeoutReached() {
  return (millis() - _connectStartTime) > (1000 * WIFI_CONNECT_TIMEOUT);
}

void PersWiFiManager::startApMode() {
  // start AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_MODE_IP, AP_MODE_IP, SUBNET);
  _apPass.length() ? WiFi.softAP(getApSsid().c_str(), _apPass.c_str()) : WiFi.softAP(getApSsid().c_str());
  if (_apHandler) _apHandler();

}  // startApMode

void PersWiFiManager::setConnectNonBlock(bool b) {
  _connectNonBlock = b;
}  // setConnectNonBlock

void PersWiFiManager::setupWiFiHandlers() {
  _dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  _dnsServer->start((byte)53, "*", AP_MODE_IP);  // used for captive portal in AP mode

  _server->on("/wifi/list", [&](AsyncWebServerRequest* request) {
    // scan for wifi networks
    int n = WiFi.scanNetworks();

    // build array of indices
    int ix[n];
    for (int i = 0; i < n; i++) ix[i] = i;

    // sort by signal strength
    for (int i = 0; i < n; i++)
      for (int j = 1; j < n - i; j++)
        if (WiFi.RSSI(ix[j]) > WiFi.RSSI(ix[j - 1])) std::swap(ix[j], ix[j - 1]);
    // remove duplicates
    for (int i = 0; i < n; i++)
      for (int j = i + 1; j < n; j++)
        if (WiFi.SSID(ix[i]).equals(WiFi.SSID(ix[j])) &&
            WiFi.encryptionType(ix[i]) == WiFi.encryptionType(ix[j])) ix[j] = -1;

    // build plain text string of wifi info
    // format [signal%]:[encrypted 0 or 1]:SSID
    String s = "";
    s.reserve(2050);
    for (int i = 0; i < n && s.length() < 2000; i++) {  // check s.length to limit memory usage
      if (ix[i] != -1) {
        s += String(i ? "\n" : "") + ((constrain(WiFi.RSSI(ix[i]), -100, -50) + 100) * 2) + "," +
             ((WiFi.encryptionType(ix[i]) == ENC_TYPE_NONE) ? 0 : 1) + "," + WiFi.SSID(ix[i]);
      }
    }

    // send string to client
    request->send(200, "text/plain", s);
  });  //_server->on /wifi/list

  _server->on("/wifi/connect", HTTP_POST, [&](AsyncWebServerRequest* request) {
    String ssid = request->arg("ssid"), pass = request->arg("pass"),
           ip = request->arg("ip"), gateway = request->arg("gateway");
    attemptConnection(ssid, pass, ip, gateway);
    request->send(200, "text/html", "connecting...");
  });  //_server->on /wifi/connect

  _server->on("/wifi/ap", [&](AsyncWebServerRequest* request) {
    request->send(200, "text/html", "access point: " + getApSsid());
    startApMode();
  });  //_server->on /wifi/ap

  _server->on("/wifi/rst", [&](AsyncWebServerRequest* request) {
    request->send(200, "text/html", "Rebooting...");
    delay(100);
    // ESP.restart();
    //  Adding Safer Restart method
    ESP.wdtDisable();
    ESP.reset();
    delay(2000);
  });

}  // setupWiFiHandlers

void PersWiFiManager::begin() {
  LittleFS.begin();
  _server->begin();
  setupWiFiHandlers();
  _server->serveStatic("/", LittleFS, "/ap")
      .setDefaultFile("wifi.html");
  _isRunning = true;
}  // begin

void PersWiFiManager::loopServers() {
  _dnsServer->processNextRequest();
}

void PersWiFiManager::stopServers() {
  LittleFS.end();
  _dnsServer->stop();
  _server->end();
  _isRunning = false;
}

bool PersWiFiManager::isRunning() {
  return _isRunning;
}

String PersWiFiManager::getApSsid() {
  if (!_apSsid.length()) {
    _apSsid = "ESP8266-AP-" + String(random());
  }
  return _apSsid;
}  // getApSsid

void PersWiFiManager::setApCredentials(const String& apSsid, const String& apPass) {
  if (apSsid.length()) _apSsid = apSsid;
  if (apPass.length() >= 8) _apPass = apPass;
}  // setApCredentials

void PersWiFiManager::onConnect(WiFiChangeHandlerFunction fn) {
  _connectHandler = fn;
}

void PersWiFiManager::onAp(WiFiChangeHandlerFunction fn) {
  _apHandler = fn;
}

void PersWiFiManager::onAttemptConnection(WiFiChangeHandlerFunction fn) {
  _attemptConnectionHandler = fn;
}
