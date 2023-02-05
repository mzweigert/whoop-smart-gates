#include <EEPROM.h>
#include <WString.h>
#include <EEPROMAdresses.h>

class EEPROMManager {
  public:
    static bool writeString(int startAddress, String writeString);
    static String readString(int startAddress, int maxLength);
    static bool writeByte(int address, uint8_t value);
    static uint8_t readByte(int address);
    
    static void init();
   
};
