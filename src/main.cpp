#include <Arduino.h>
#include <RGBPins.h>
#include <LedStrip.h>
#include <EEPROMManager.h>
#include <WiFiConnector.h>
#include <LedStripWebServer.h>

WiFiConnector *wiFiConnector;
LedStripWebServer* ledStripWebServer ;

void setup() {
  Serial.begin(115200);
  while (! Serial); 
  delay(10000);
  EEPROMManager::init();
  wiFiConnector = new WiFiConnector();
  ledStripWebServer = new LedStripWebServer();;
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
}
