#include <Arduino.h>
#include <Controller.h>
#include <Model.h>
#include <TimeLib.h>
#include <View.h>

#define SS_SWITCH 24
#define SS_ADDR 0x36

const char *PREFERENCES_NS = "fish";
long DEFAULT_SUNRISE_LENGTH = 60;  // Minutes
long DEFAULT_SUNSET_LENGTH = 60;   // Minutes
long DEFAULT_MAX_BRIGHTNESS = 100; // Percent
time_t DEFAULT_MANUAL_SUNRISE;
time_t DEFAULT_MANUAL_SUNSET;

View view;
Model model;
Controller controller;

time_t createTime(uint8_t hour, uint8_t minute);

void setup() {
  Serial.begin(115200);

  DEFAULT_MANUAL_SUNRISE = createTime(8, 0);
  DEFAULT_MANUAL_SUNSET = createTime(18, 0);

  // TODO: change these with rtc and calculation
  model.currentTime = createTime(5, 20);
  model.sunriseTime = createTime(5, 20);
  model.sunsetTime = createTime(5, 20);

  model.init(PREFERENCES_NS, DEFAULT_SUNRISE_LENGTH, DEFAULT_SUNSET_LENGTH,
             DEFAULT_MAX_BRIGHTNESS, DEFAULT_MANUAL_SUNRISE,
             DEFAULT_MANUAL_SUNSET);
  view.init();
  controller.init(SS_ADDR, SS_SWITCH);
  delay(1000);
}

void loop() { controller.run(model, view); }

time_t createTime(uint8_t hour, uint8_t minute) {
  uint8_t year = 53;
  TimeElements tm = {1, minute, hour, 1, 22, 5, year};
  return makeTime(tm);
}
