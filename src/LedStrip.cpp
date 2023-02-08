#include "LedStrip.h"

void LedStrip::setPinAsDigitalOutput(uint8_t pin) {
    if (pin == RX || pin == TX){
        pinMode(pin, FUNCTION_3);
    }
    pinMode(pin, OUTPUT);
}

void LedStrip::rgb(uint8_t red, uint8_t green, uint8_t blue) {
    analogWrite(this->redPin, red);
    analogWrite(this->greenPin, green);
    analogWrite(this->bluePin, blue);
}

uint8_t LedStrip::red() {
    return map(analogRead(this->redPin), 0, 1023, OFF, ON);
}

uint8_t LedStrip::green() {
    return map(analogRead(this->greenPin), 0, 1023, OFF, ON);
}

uint8_t LedStrip::blue() {
    return map(analogRead(this->bluePin), 0, 1023, OFF, ON);
}

void LedStrip::disable(){
    rgb(OFF, OFF, OFF);
}

LedStrip::LedStrip(uint8_t redPin, uint8_t greenPin, uint8_t bluePin) {
    setPinAsDigitalOutput(redPin);
    setPinAsDigitalOutput(greenPin);
    setPinAsDigitalOutput(bluePin);
    this->redPin = redPin;
    this->greenPin = greenPin;
    this->bluePin = bluePin;

}