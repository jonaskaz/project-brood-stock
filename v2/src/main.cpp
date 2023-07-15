#include "Controller.h"
#include "Dimmer.h"
#include "Model.h"
#include "View.h"
#include <Arduino.h>
#include <TimeLib.h>

// Controller parameters
const int SS_SWITCH = 24;
const int SS_ADDR = 0x36;
const char *PREFERENCES_NS = "fish";
const long DEFAULT_SUNRISE_LENGTH = 60;  // Minutes
const long DEFAULT_SUNSET_LENGTH = 60;   // Minutes
const long DEFAULT_MAX_BRIGHTNESS = 100; // Percent
time_t DEFAULT_MANUAL_SUNRISE;
time_t DEFAULT_MANUAL_SUNSET;

// Dimmer parameters
const int MAXBRIGHTNESS = 4095;
const int MINBRIGHTNESS = 300;
const long SUNRISELENGTHSECONDS = 3600L;
const long SUNSETLENGTHSECONDS = 3600L;
const double LATITUDE = 46.8421690;
const double LONGITUDE = -88.3803090;
const double TIMEZONE = -5.0;
const MCP4728_channel_t DACCHANNEL = MCP4728_CHANNEL_A;

View view;
Model model;
Controller controller;
Dimmer dimmer;

time_t createTime(uint8_t hour, uint8_t minute);

void setup() {
  Serial.begin(115200);

  // TODO: replace these with rtc
  DEFAULT_MANUAL_SUNRISE = createTime(8, 0);
  DEFAULT_MANUAL_SUNSET = createTime(18, 0);
  model.currentTime = createTime(7, 40);

  model.init(PREFERENCES_NS, DEFAULT_SUNRISE_LENGTH, DEFAULT_SUNSET_LENGTH,
             DEFAULT_MAX_BRIGHTNESS, DEFAULT_MANUAL_SUNRISE,
             DEFAULT_MANUAL_SUNSET);
  view.init();
  controller.init(SS_ADDR, SS_SWITCH);
  dimmer.init(MAXBRIGHTNESS, MINBRIGHTNESS, SUNRISELENGTHSECONDS,
              SUNSETLENGTHSECONDS, LATITUDE, LONGITUDE, TIMEZONE, DACCHANNEL);
  delay(1000);
}

void loop() {
  controller.run(model, view);
  dimmer.run(model);
}

time_t createTime(uint8_t hour, uint8_t minute) {
  uint8_t year = 53;
  TimeElements tm = {1, minute, hour, 6, 15, 7, year};
  return makeTime(tm);
}
