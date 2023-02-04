#include <Arduino.h>
#include <RGBPins.h>
#include <LedStrip.h>
#include <WiFiConnector.h>

int time_del = 1000;
void setup() {
  Serial.begin(9600);
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
  boolean connected = WiFiConnector::connectToWiFi();
  if (connected) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop() {
   WiFiConnector::loop();
   boolean connected = WiFiConnector::isConnected();
   if (connected && digitalRead(BUILTIN_LED)) {
     digitalWrite(LED_BUILTIN, LOW);
   }
   
}
