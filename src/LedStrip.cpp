#include "LedStrip.h"

void LedStrip::setPinAsDigitalOutput(uint8_t pin) {
    if (pin == RX || pin == TX){
        pinMode(pin, FUNCTION_3);
    }
    pinMode(pin, OUTPUT);
}

void LedStrip::red(uint8_t value) {
    analogWrite(redPin, value);
}

void LedStrip::green(uint8_t value) {
     analogWrite(greenPin, value);
}

void LedStrip::blue(uint8_t value) {
     analogWrite(bluePin, value);
}

void LedStrip::disable(){
    digitalWrite(redPin, OFF);
    digitalWrite(greenPin, OFF);
    digitalWrite(bluePin, OFF);
}

LedStrip::LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin) {
    setPinAsDigitalOutput(redPin);
    setPinAsDigitalOutput(greenPin);
    setPinAsDigitalOutput(bluePin);
    this->redPin = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;

}