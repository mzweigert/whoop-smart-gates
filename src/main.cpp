#include <Arduino.h>
#include <RGBPins.h>
#include <LedStrip.h>
#include <EEPROMManager.h>
#include <WiFiConnector.h>
#include <LedStripWebServer.h>

WiFiConnector *wiFiConnector = new WiFiConnector();
LedStripWebServer* ledStripWebServer = new LedStripWebServer();

void setup() {
  Serial.begin(115200);
  while (! Serial); 
  delay(5000);
  EEPROMManager::init();
}

void loop() {
   if(!WiFi.isConnected() || wiFiConnector->status() == INITIALIZING || wiFiConnector->status() == IN_AP_MODE) {
      if(ledStripWebServer->isRunning()) {
        ledStripWebServer->stop();
        Serial.println("led server stop");
      }
      wiFiConnector->loop();
   } else {
      if(wiFiConnector->canStopAPMode()) {
        wiFiConnector->stop();
        Serial.println("AP Mode stop");
      }
      if(!ledStripWebServer->isRunning()) {
        Serial.println("led server begin");
        ledStripWebServer->begin();
      }
   }
   
}
