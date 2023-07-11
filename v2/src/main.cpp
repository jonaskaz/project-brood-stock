#include "Wire.h"
#include <Adafruit_LiquidCrystal.h>
#include <Arduino.h>
#include <Controller.h>
#include <Model.h>
#include <TimeLib.h>
#include <View.h>

#define SS_SWITCH 24
#define SS_ADDR 0x36

View view;
Model m;
Controller controller(SS_ADDR, SS_SWITCH);

void setup() {
  Serial.begin(115200);
  uint8_t year = 1;
  TimeElements tm = {1, 20, 14, 1, 10, 5, year};
  m.sunriseTime = makeTime(tm);
  m.sunsetTime = makeTime(tm);
  m.manualTiming = false;
  view.init();
}

void loop() {
  view.currentScreen = manualMode;
  view.showScreen(m);
  delay(3000);
  view.currentScreen = home;
  view.showScreen(m);
  delay(3000);
}
