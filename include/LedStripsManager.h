#ifndef LedStripManager_h
#define LedStripManager_h

#include <inttypes.h>
#include <optional>
#include <Arduino.h>
#include <RGBPins.h>
#include <LedStrip.h>
#include <EEPROMManager.h>

#define SIZEOF(arr) sizeof(arr) / sizeof(*arr)

#define LED_STRIPS_NUMBER 3

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
    std::array<LedStrip*, LED_STRIPS_NUMBER> ledStrips;
    std::array<LedStrip*, LED_STRIPS_NUMBER> initPinsMap();

public:
    void initColors();
    bool changeColor(String id, uint8_t red, uint8_t green, uint8_t blue);
    
    void enable();
    void disable();

    bool isEnabled();

    std::array<String, LED_STRIPS_NUMBER> getLedStripsIds();
    std::optional<Colors> getSavedColorsForLedStripById(String id);

    LedStripsManager();
};
#endif
