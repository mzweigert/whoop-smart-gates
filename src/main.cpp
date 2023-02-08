#include <Arduino.h>
#include <RGBPins.h>
#include <LedStrip.h>
#include <EEPROMManager.h>
#include <WiFiConnector.h>
#include <LedStripWebServer.h>

WiFiConnector *wiFiConnector = new WiFiConnector();
LedStripWebServer* ledStripWebServer;

void setup() {
  Serial.begin(9600);
  EEPROMManager::init();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  boolean connected = wiFiConnector->connectToWiFi();
  if (connected) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop() {
   if(wiFiConnector != NULL) {
    wiFiConnector->loop();
    boolean connected = wiFiConnector->isConnected();
    if (connected) {
     digitalWrite(LED_BUILTIN, LOW);
     wiFiConnector->~WiFiConnector();
     wiFiConnector = NULL;
    }
   } else if(ledStripWebServer == NULL) {
    ledStripWebServer = new LedStripWebServer();
   } else {
    ledStripWebServer->loop();
   }
   
}
