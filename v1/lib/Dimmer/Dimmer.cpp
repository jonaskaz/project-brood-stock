#include "Dimmer.h"
#include <Arduino.h>
#include <TimeLib.h>

Dimmer::Dimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
               long sunsetLenSeconds) {
  maxBright = maxBrightness;
  minBright = minBrightness;
  sunriseSeconds = sunriseLenSeconds;
  sunsetSeconds = sunsetLenSeconds;
  brightness = minBright;
  startBrightness = brightness;
}

TimeElements Dimmer::createTimeElements(RV8803 rtc) {
  TimeElements tm = {rtc.getSeconds(),     rtc.getMinutes(), rtc.getHours(),
                     rtc.getWeekday() + 1, rtc.getDate(),    rtc.getMonth(),
                     rtc.getYear()};
  return tm;
};

void Dimmer::setStartTime(TimeElements tm) { startTime = makeTime(tm); }

unsigned long Dimmer::updateElapsedTime(TimeElements tm) {
  time_t currentTime = makeTime(tm);
  totalElapsedSeconds = currentTime - startTime;
  return totalElapsedSeconds;
}

void Dimmer::updateSunriseBrightness() {
  unsigned long brightnessBuffer =
      map(startBrightness, minBright, maxBright, 0, sunriseSeconds);
  brightness = min(maxBright, map(totalElapsedSeconds + brightnessBuffer, 0,
                                  sunriseSeconds, minBright, maxBright));
}

void Dimmer::updateSunsetBrightness() {
  unsigned long brightnessBuffer =
      map(startBrightness, maxBright, minBright, 0, sunsetSeconds);
  brightness = max(minBright, map(totalElapsedSeconds + brightnessBuffer,
                                  sunsetSeconds, 0, minBright, maxBright));
}
