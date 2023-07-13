#include <Arduino.h>
#include <Controller.h>
#include <Model.h>
#include <TimeLib.h>
#include <View.h>

#define SS_SWITCH 24
#define SS_ADDR 0x36

View view;
Model m;
Controller controller;

void setup() {
  Serial.begin(115200);

  // Set inital times for testing
  uint8_t year = 1;
  TimeElements tm = {1, 20, 4, 1, 10, 5, year};
  m.currentTime = makeTime(tm);
  m.sunriseTime = makeTime(tm);
  m.sunsetTime = makeTime(tm);
  m.manualSunriseTime = makeTime(tm);
  m.manualSunsetTime = makeTime(tm);

  view.init();
  controller.init(SS_ADDR, SS_SWITCH);
}

void loop() { controller.run(m, view); }
