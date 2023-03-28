#ifndef LedStrip_h
#define LedStrip_h

#include <inttypes.h>
#include <Arduino.h>
#include <RGBPins.h>

enum Status {
  ON = 255, OFF = 0
};

enum Color {
  RED = 0,
  GREEN = 1,
  BLUE = 2
};

class LedStrip {
private:
  uint8_t _order, redPin, greenPin, bluePin;
  void setPinAsDigitalOutput(uint8_t pin);

public:
  void set(uint8_t red, uint8_t green, uint8_t blue);
  uint8_t order();
  uint8_t red();
  uint8_t green();
  uint8_t blue();
  void disable();


  LedStrip(uint8_t order, uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
};

#endif