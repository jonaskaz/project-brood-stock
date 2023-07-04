#include "Dimmer.h"
#include <Arduino.h>

Dimmer::Dimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
               long sunsetLenSeconds) {
  maxBright = maxBrightness;
  minBright = minBrightness;
  sunriseSeconds = sunriseLenSeconds;
  sunsetSeconds = sunsetLenSeconds;
}

void Dimmer::setStartTime(int hour, int minute, int second) {
  startHour = hour;
  startMinute = minute;
  startSecond = second;
}

unsigned long Dimmer::updateElapsedTime(int currentHour, int currentMinute,
                                        int currentSecond) {
  int elapsedHour = currentHour - startHour;
  if (currentHour < startHour) {
    elapsedHour = (24 - startHour) + currentHour;
  }
  int elapsedMinutes = currentMinute - startMinute;
  if (currentMinute < startMinute) {
    elapsedMinutes = (60 - startMinute) + currentMinute;
  }
  int elapsedSeconds = currentSecond - startSecond;
  if (currentSecond < startSecond) {
    elapsedSeconds = (60 - startSecond) + currentSecond;
  }
  totalElapsedSeconds =
      (elapsedHour * 3600) + (elapsedMinutes * 60) + elapsedSeconds;
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
