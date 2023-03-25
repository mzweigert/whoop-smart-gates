#include <DeviceReset.h>

void DeviceReset::reset() {
  pinMode(WAKE_UP, OUTPUT);
  digitalWrite(WAKE_UP, LOW);
}