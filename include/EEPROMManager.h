
#ifndef EEPROMManager_h
#define EEPROMManager_h

#include <EEPROM.h>
#include <WString.h>
#include <EEPROMAdresses.h>

#include <Arduino.h>

class EEPROMManager {
public:
  static bool writeString(int startAddress, int endAddress, String writeString);
  static String readString(int startAddress, int size);
  static bool writeBytes(int address, uint8_t* bytes, int arraySize);
  static uint8_t* readBytes(int startAddress, int size, uint8_t* destination);
  static bool writeByte(int address, uint8_t value);
  static uint8_t readByte(int address);

  static void init();
};
#endif
