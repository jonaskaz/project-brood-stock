#include "Wire.h"
#include <Adafruit_LiquidCrystal.h>
#include <Arduino.h>
#include <Model.h>
#include <TimeLib.h>
#include <View.h>

View view;
Model m;

void setup() {
  Serial.begin(115200);
  uint8_t year = 1;
  TimeElements tm = {1, 2, 3, 1, 10, 5, year};
  m.sunriseTime = makeTime(tm);
  m.sunsetTime = makeTime(tm);
  m.manualTiming = false;
  view.init();
}

void loop() {
  Screen screen;
  screen = manualMode;
  view.showScreen(screen, m);
  delay(3000);
  screen = home;
  view.showScreen(screen, m);
  delay(3000);
}
