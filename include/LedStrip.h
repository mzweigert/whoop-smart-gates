#ifndef LedStrip_h
#define LedStrip_h

#include <inttypes.h>
#include <Arduino.h>

enum Status {
    ON = 255, OFF = 0
};

class LedStrip {
 private:
  uint8_t redPin, greenPin, bluePin;
  void setPinAsDigitalOutput(uint8_t pin);

 public:
  void rgb(uint8_t red, uint8_t green, uint8_t blue);
  uint8_t red();
  uint8_t green();
  uint8_t blue();
  void disable();

  LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
};

#endif