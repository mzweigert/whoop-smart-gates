#include <EEPROMManager.h>

void EEPROMManager::init() {
    EEPROM.begin(EEPROM_SIZE);
}

bool EEPROMManager::writeString(int startAddress, String writeString) {
    for (unsigned int i = 0; i < writeString.length() ; ++i){
      EEPROM.write(startAddress + i, writeString[i]);
    }
    return EEPROM.commit();
}

String EEPROMManager::readString(int startAddress, int endAddress) {
  String value = "";
  for (int i = startAddress; i < startAddress + endAddress; ++i){
    uint8_t saved = EEPROM.read(i);
    if(saved == 255) break;
    value += char(saved);
  }

  return value;
}

bool EEPROMManager::writeByte(int address, uint8_t value) {
    EEPROM.write(address, value);
    return EEPROM.commit();
}

uint8_t EEPROMManager::readByte(int address) {
    return EEPROM.read(address);
}