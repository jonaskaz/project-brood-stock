#include "Controller.h"
#include "Dimmer.h"
#include "Model.h"
#include "View.h"
#include <Arduino.h>
#include <RTClib.h>
#include <TimeLib.h>

// Controller parameters
const int SS_SWITCH = 24;
const int SS_ADDR = 0x36;
const char *PREFERENCES_NS = "fish";
const long DEFAULT_SUNRISE_LENGTH = 60;  // Minutes
const long DEFAULT_SUNSET_LENGTH = 60;   // Minutes
const long DEFAULT_MAX_BRIGHTNESS = 100; // Percent
const int DEFAULT_MANUAL_SUNRISE_HOUR = 8;
const int DEFAULT_MANUAL_SUNRISE_MINUTE = 0;
const int DEFAULT_MANUAL_SUNSET_HOUR = 18;
const int DEFAULT_MANUAL_SUNSET_MINUTE = 0;

// Dimmer parameters
const int MAXBRIGHTNESS = 4095;
const int MINBRIGHTNESS = 300;
const long SUNRISELENGTHSECONDS = 3600L;
const long SUNSETLENGTHSECONDS = 3600L;
const double LATITUDE = 46.8421690;
const double LONGITUDE = -88.3803090;
const double TIMEZONE = -4.0;
const MCP4728_channel_t DACCHANNEL = MCP4728_CHANNEL_A;

View view;
Model model;
Controller controller;
Dimmer dimmer;
RTC_DS3231 rtc;

time_t createTime(uint8_t hour, uint8_t minute);
void setupRTC();

void setup() {
  Serial.begin(115200);
  setupRTC();
  time_t DEFAULT_MANUAL_SUNRISE =
      createTime(DEFAULT_MANUAL_SUNRISE_HOUR, DEFAULT_MANUAL_SUNRISE_MINUTE);
  time_t DEFAULT_MANUAL_SUNSET =
      createTime(DEFAULT_MANUAL_SUNSET_HOUR, DEFAULT_MANUAL_SUNSET_MINUTE);
  model.currentTime = rtc.now().unixtime();
  model.init(PREFERENCES_NS, DEFAULT_SUNRISE_LENGTH, DEFAULT_SUNSET_LENGTH,
             DEFAULT_MAX_BRIGHTNESS, DEFAULT_MANUAL_SUNRISE,
             DEFAULT_MANUAL_SUNSET);
  view.init();
  controller.init(SS_ADDR, SS_SWITCH);
  dimmer.init(MAXBRIGHTNESS, MINBRIGHTNESS, SUNRISELENGTHSECONDS,
              SUNSETLENGTHSECONDS, LATITUDE, LONGITUDE, TIMEZONE, DACCHANNEL,
              model.currentTime);
  dimmer.setModelSunriseSunsetTime(model);
}

void loop() {
  model.currentTime = rtc.now().unixtime();
  controller.run(model, view);
  dimmer.run(model);
}

time_t createTime(uint8_t hour, uint8_t minute) {
  DateTime now = rtc.now();
  uint8_t year = now.year() - 1970;
  TimeElements tm = {1,         minute,      hour, now.dayOfTheWeek(),
                     now.day(), now.month(), year};
  return makeTime(tm);
}

void setupRTC() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      delay(10);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}
