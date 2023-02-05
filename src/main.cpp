#include <Arduino.h>
#include <RGBPins.h>
#include <LedStrip.h>
#include <WiFiConnector.h>

int time_del = 1000;
WiFiConnector *wiFiConnector = new WiFiConnector();
void setup() {
  Serial.begin(9600);
  EEPROMManager::init();
  LedStrip* firstStrip = new LedStrip(FIRST_RED, FIRST_GREEN, FIRST_BLUE);
  LedStrip* secondStrip = new LedStrip(SECOND_RED, SECOND_GREEN, SECOND_BLUE);
  LedStrip* thirdStrip = new LedStrip(THIRD_RED, THIRD_GREEN, THIRD_BLUE);

  firstStrip->disable();
  secondStrip->disable();
  thirdStrip->disable();

  firstStrip->red(ON);
  firstStrip->green(ON);
  firstStrip->blue(ON);

  secondStrip->red(ON);
  secondStrip->green(ON);
  secondStrip->blue(ON);

  secondStrip->red(ON);
  secondStrip->green(ON);
  secondStrip->blue(ON);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(time_del * 3);
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
   } else {

   }
   
}
