#include <Arduino.h>
#include <EEPROMManager.h>
#include <ESP8266mDNS.h>
#include <LedStripsWebServer.h>
#include <LedStripsManager.h>
#include <WiFiConnector.h>

LedStripsManager *ledStripsManager;
WiFiConnector *wiFiConnector;
LedStripsWebServer *ledStripsWebServer;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(5000);
  ledStripsManager = new LedStripsManager();
  MDNS.addService("http", "tcp", 80);
  wiFiConnector = new WiFiConnector(ledStripsManager);
  ledStripsWebServer = new LedStripsWebServer(ledStripsManager);
  wdt_disable();
}

void loop() {
  if (!WiFi.isConnected() || wiFiConnector->status() != CONNECTED) {
    if (ledStripsWebServer->isRunning()) {
      Serial.println("led server shutdown...");
      ledStripsWebServer->stop();
    }
    wiFiConnector->loop();
  } else {
    if (!ledStripsWebServer->isRunning()) {
      Serial.println("led server begin");
      ledStripsWebServer->begin();
    }
    ledStripsWebServer->loop();
  }
  wdt_reset();
}
