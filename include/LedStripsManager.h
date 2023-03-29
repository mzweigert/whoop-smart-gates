#ifndef LedStripManager_h
#define LedStripManager_h

#include <inttypes.h>
#include <map>
#include <optional>
#include <Arduino.h>
#include <RGBPins.h>
#include <LedStrip.h>
#include <EEPROMManager.h>

#define SIZEOF(arr) sizeof(arr) / sizeof(*arr)

#define LED_STRIPS_NUMBER 3

#define BLINK_MAX_POWER (ON / 4)
#define BLINK_MS 500

class Colors {
private:
  uint8_t red;
  uint8_t green;
  uint8_t blue;
public:
  uint8_t getRed();
  uint8_t getGreen();
  uint8_t getBlue();

  Colors(uint8_t red, uint8_t green, uint8_t blue);

};

class LedStripsManager {
private:
  std::map<uint8_t, LedStrip*> ledStrips;
  unsigned long blinkSwitchTime;
  bool blinkEnable;

  std::map<uint8_t, LedStrip*> initPinsMap();

  void switchBlink(Color color, uint8_t power);



public:
  void initColors();
  bool changeColor(uint8_t order, uint8_t red, uint8_t green, uint8_t blue);
  void changeColors(uint8_t red, uint8_t green, uint8_t blue);
  void blink(Color color);

  void enable();
  void disable();

  bool isEnabled();

  std::array<uint8_t, LED_STRIPS_NUMBER> getLedStripsOrders();
  std::optional<Colors> getSavedColorsForLedStripByOrder(uint8_t order);

  LedStripsManager();
};
#endif
