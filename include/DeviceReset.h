#ifndef DeviceReset_h
#define DeviceReset_h

#include <inttypes.h>
#include <Arduino.h>
#include <RGBPins.h>

#define WAKE_UP 16

class DeviceReset {
public:
    static void reset();
};

#endif