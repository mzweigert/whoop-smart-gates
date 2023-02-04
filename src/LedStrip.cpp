#include "LedStrip.h"

void LedStrip::setPinAsDigitalOutput(uint8_t pin) {
    if (pin == RX || pin == TX){
        pinMode(pin, FUNCTION_3);
    }
    pinMode(pin, OUTPUT);
}

void LedStrip::red(Status status) {
    digitalWrite(redPin, status);
}

void LedStrip::green(Status status) {
     digitalWrite(greenPin, status);
}

void LedStrip::blue(Status status) {
     digitalWrite(bluePin, status);
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