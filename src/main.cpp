#include <Arduino.h>
#include <EEPROMManager.h>
#include <LedStripsManager.h>
#include <WiFiConnector.h>
#include <LedStripWebServer.h>

LedStripsManager *ledStripsManager;
WiFiConnector *wiFiConnector;
LedStripWebServer* ledStripWebServer;

void setup() {
  Serial.begin(115200);
  while (! Serial); 
  delay(10000);
  EEPROMManager::init();
  ledStripsManager = new LedStripsManager();
  wiFiConnector = new WiFiConnector();
  ledStripWebServer = new LedStripWebServer(ledStripsManager);
  wdt_disable();
}

void loop() {
   if(!WiFi.isConnected() || wiFiConnector->status() != CONNECTED) {
      if(ledStripWebServer->isRunning()) {
        Serial.println("led server shutdown...");
        ledStripWebServer->stop();
      }
      wiFiConnector->loop();
   } else {
      if(!ledStripWebServer->isRunning()) {
        Serial.println("led server begin");
        ledStripWebServer->begin();
      }
   }
   wdt_reset();
}
