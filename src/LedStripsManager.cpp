#include <LedStripsManager.h>

uint8_t Colors::getRed() {
  return red;
}

uint8_t Colors::getGreen() {
  return green;
}

uint8_t Colors::getBlue() {
  return blue;
}

Colors::Colors(uint8_t red, uint8_t green, uint8_t blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
}

const static std::array<std::array<uint8_t, 3>, LED_STRIPS_NUMBER> ledStripsPins{
    {{FIRST_RED, FIRST_GREEN, FIRST_BLUE},     // FIRST
     {SECOND_RED, SECOND_GREEN, SECOND_BLUE},  // SECOND
     {THIRD_RED, THIRD_GREEN, THIRD_BLUE}}     // THIRD
};

std::map<uint8_t, LedStrip*> LedStripsManager::initPinsMap() {
  std::map<uint8_t, LedStrip*> leds;
  for (int i = 0; i < ledStripsPins.size(); i++) {
    leds[i] = new LedStrip(i, ledStripsPins[i][0], ledStripsPins[i][1], ledStripsPins[i][2]);
  }
  return leds;
}

void LedStripsManager::initColors() {
  uint8_t enabled = EEPROMManager::readByte(ENABLED_ADDRESS);
  for (int i = 0; i < ledStrips.size(); i++) {
    if (enabled) {
      uint8_t bytes[3];
      uint8_t* colors = EEPROMManager::readBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (i * 3), 3, bytes);
      ledStrips[i]->set(*(colors), *(colors + 1), *(colors + 2));
    } else {
      ledStrips[i]->disable();
    }
  }
}

bool LedStripsManager::changeColor(uint8_t order, uint8_t red, uint8_t green, uint8_t blue) {
  if (ledStrips.find(order) != ledStrips.end()) {
    uint8_t colors[] = {red, green, blue};
    EEPROMManager::writeBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (order * 3), colors, SIZEOF(colors));
    uint8_t enabled = EEPROMManager::readByte(ENABLED_ADDRESS);
    if (enabled) {
      ledStrips[order]->set(red, green, blue);
    }
    return true;
  }
  return false;
}

void LedStripsManager::changeColors(uint8_t red, uint8_t green, uint8_t blue) {
  for (int i = 0; i < ledStrips.size(); i++) {
    ledStrips[i]->set(red, green, blue);
  }
}

void LedStripsManager::blink(Color color) {
  if (blinkSwitchTime + BLINK_MS <= millis()) {
    blinkEnable = !blinkEnable;
    uint8_t power = blinkEnable ? BLINK_MAX_POWER : 0;
    switchBlink(color, power);
    blinkSwitchTime = millis();
  }
}

void LedStripsManager::switchBlink(Color color, uint8_t power) {
  switch (color) {
    case RED:
      changeColors(power, 0, 0);
      break;
    case GREEN:
      changeColors(0, power, 0);
      break;
    case BLUE:
      changeColors(0, 0, power);
      break;
  }
}

void LedStripsManager::enable() {
  for (int i = 0; i < ledStrips.size(); i++) {
    uint8_t bytes[3];
    uint8_t* colors = EEPROMManager::readBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (i * 3), 3, bytes);
    ledStrips[i]->set(*(colors), *(colors + 1), *(colors + 2));
  }
  EEPROMManager::writeByte(ENABLED_ADDRESS, ON);
}

void LedStripsManager::disable() {
  for (int i = 0; i < ledStrips.size(); i++) {
    ledStrips[i]->disable();
  }
  EEPROMManager::writeByte(ENABLED_ADDRESS, OFF);
}

bool LedStripsManager::isEnabled() {
  return EEPROMManager::readByte(ENABLED_ADDRESS) > 0;
}

std::array<uint8_t, LED_STRIPS_NUMBER> LedStripsManager::getLedStripsOrders() {
  std::array<uint8_t, LED_STRIPS_NUMBER> ids;
  for (int i = 0; i < ledStrips.size(); i++) {
    ids[i] = ledStrips[i]->order();
  }
  return ids;
}

std::optional<Colors> LedStripsManager::getSavedColorsForLedStripByOrder(uint8_t order) {
  if (ledStrips.find(order) != ledStrips.end()) {
    uint8_t bytes[3];
    uint8_t* colors = EEPROMManager::readBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (order * 3), 3, bytes);
    return Colors(*(colors), *(colors + 1), *(colors + 2));
  }
  return std::nullopt;
}

LedStripsManager::LedStripsManager() {
  this->ledStrips = initPinsMap();
  disable();
}