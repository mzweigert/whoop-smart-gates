#include <EEPROMManager.h>

void EEPROMManager::init() {
    EEPROM.begin(EEPROM_SIZE);
}

bool EEPROMManager::writeString(int startAddress, int endAddress, String writeString) {
    int sizeToWrite = endAddress - startAddress;
    if (startAddress >= endAddress || writeString.length() > sizeToWrite) {
      return false;
    }
    
    for (unsigned int i = 0; i < sizeToWrite ; ++i){
      if (i < writeString.length()) {
         EEPROM.write(startAddress + i, writeString[i]);
      } else {
        EEPROM.write(startAddress + i, 0);
      }
    }
    return EEPROM.commit();
}

String EEPROMManager::readString(int startAddress, int maxLength) {
  String value = "";
  for (int i = 0; i < maxLength; ++i){
    uint8_t saved = EEPROM.read(startAddress + i);
    if(saved < 32 || saved == 255) {
      break;
    }
    value += char(saved);
  }

  return value;
}

bool EEPROMManager::writeBytes(int address, uint8_t* bytes, int arraySize) {
    for (uint8_t i = 0; i < arraySize ; i++) {
         EEPROM.write(address + i, *(bytes + i));
    }
    return EEPROM.commit();
}

uint8_t* EEPROMManager::readBytes(int startAddress, int size, uint8_t* destination) {
  for (int i = 0; i < size; i++){
    *(destination + i) = EEPROM.read(startAddress + i);
  }
  return destination;
}

bool EEPROMManager::writeByte(int address, uint8_t value) {
    EEPROM.write(address, value);
    return EEPROM.commit();
}

uint8_t EEPROMManager::readByte(int address) {
    return EEPROM.read(address);
}