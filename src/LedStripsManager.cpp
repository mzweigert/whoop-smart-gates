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

const static std::array<std::array<uint8_t, 3>, LED_STRIPS_NUMBER> ledStripsPins {
    {{FIRST_RED, FIRST_GREEN, FIRST_BLUE},  // FIRST
        { SECOND_RED, SECOND_GREEN, SECOND_BLUE },  // SECOND
        { THIRD_RED, THIRD_GREEN, THIRD_BLUE }}   // THIRD
};

std::array<LedStrip*, LED_STRIPS_NUMBER> LedStripsManager::initPinsMap() {
    std::array<LedStrip*, LED_STRIPS_NUMBER> leds;
    for (int i = 0; i < ledStripsPins.size(); i++) {
        Serial.println(ledStripsPins[i][0]);
        leds[i] = new LedStrip("LS" + String(i), ledStripsPins[i][0], ledStripsPins[i][1], ledStripsPins[i][2]);
    }
    return leds;
}

void LedStripsManager::initColors() {
    uint8_t enabled = EEPROMManager::readByte(ENABLED_ADDRESS);

    if (!enabled) return;

    for (int i = 0; i < ledStrips.size(); i++) {
        uint8_t bytes[3];
        uint8_t* colors = EEPROMManager::readBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (i * 3), 3, bytes);
        ledStrips[i]->rgb(*(colors), *(colors + 1), *(colors + 2));
    }
}

bool LedStripsManager::changeColor(String id, uint8_t red, uint8_t green, uint8_t blue) {
    for (int i = 0; i < ledStrips.size(); i++) {
        if (ledStrips[i]->getId().equals(id)) {
            uint8_t colors[] = { red, green, blue };
            EEPROMManager::writeBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (i * 3), colors, SIZEOF(colors));
            uint8_t enabled = EEPROMManager::readByte(ENABLED_ADDRESS);
            if (enabled) {
                ledStrips[i]->rgb(red, green, blue);
            }
            return true;
        }
    }

    return false;
}

void LedStripsManager::enable() {
    for (int i = 0; i < ledStrips.size(); i++) {
        uint8_t bytes[3];
        uint8_t* colors = EEPROMManager::readBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (i * 3), 3, bytes);
        ledStrips[i]->rgb(*(colors), *(colors + 1), *(colors + 2));
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

std::array<String, LED_STRIPS_NUMBER> LedStripsManager::getLedStripsIds() {
    std::array<String, LED_STRIPS_NUMBER> ids;
    for (int i = 0; i < ledStrips.size(); i++) {
        ids[i] = ledStrips[i]->getId();
    }
    return ids;
}

std::optional<Colors> LedStripsManager::getSavedColorsForLedStripById(String id) {
    for (int i = 0; i < ledStrips.size(); i++) {
        LedStrip* ledStrip = ledStrips[i];
        if (ledStrip->getId().equals(id)) {
            uint8_t bytes[3];
            uint8_t* colors = EEPROMManager::readBytes(SAVED_COLORS_LED_STRIPS_ADDRESS + (i * 3), 3, bytes);
            return Colors(*(colors), *(colors + 1), *(colors + 2));
        }
    }
    return std::nullopt;
}

LedStripsManager::LedStripsManager() {
    this->ledStrips = initPinsMap();
}