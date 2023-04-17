#ifndef DeviceReset_h
#define DeviceReset_h

#include <inttypes.h>
#include <Arduino.h>
#include <RGBPins.h>

#define WAKE_UP 16
#define RESET_AP_MODE_TIME 900000UL //15 minutes
#define RESET_STA_MODE_TIME 43200000L //12 hours

class DeviceReset {
public:
    static bool const shouldReset = true;
    static void reset();
};

#endif