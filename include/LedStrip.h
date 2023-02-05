#ifndef LedStrip_h
#define LedStrip_h

#include <ESP8266WiFi.h>
#include <Wire.h>

enum Status {
    ON = 255, OFF = 0
};

class LedStrip {
 private:
  uint8_t redPin, greenPin, bluePin;
  void setPinAsDigitalOutput(uint8_t pin);

 public:
  void red(uint8_t value);
  void green(uint8_t value);
  void blue(uint8_t value);
  void disable();

  LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
};

#endif