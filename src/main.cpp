#include <Arduino.h>
#include <RGBPins.h>
#include <LedStrip.h>
#include <iostream>
#include <list>

int time_del = 1000;
void setup() {

  LedStrip* firstStrip = new LedStrip(FIRST_RED, FIRST_GREEN, FIRST_BLUE);
  LedStrip* secondStrip = new LedStrip(SECOND_RED, SECOND_GREEN, SECOND_BLUE);
  LedStrip* thirdStrip = new LedStrip(THIRD_RED, THIRD_GREEN, THIRD_BLUE);

  firstStrip->disable();
  secondStrip->disable();
  thirdStrip->disable();

  firstStrip->red(ON);
  firstStrip->green(ON);
  firstStrip->blue(ON);

  secondStrip->red(ON);
  secondStrip->green(ON);
  secondStrip->blue(ON);

  secondStrip->red(ON);
  secondStrip->green(ON);
  secondStrip->blue(ON);

}

void loop() {

}