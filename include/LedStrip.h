#ifndef LedStrip_h
#define LedStrip_h

#include <ESP8266WiFi.h>
#include <Wire.h>

enum Status {
    ON = HIGH, OFF = LOW
};

class LedStrip {
 private:
  uint8_t redPin, greenPin, bluePin;
  void setPinAsDigitalOutput(uint8_t pin);

 public:
  void red(Status status);
  void green(Status status);
  void blue(Status status);
  void disable();

  LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
};

#endif