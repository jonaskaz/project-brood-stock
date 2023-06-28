#include "Dimmer.h"
#include <Arduino.h>

Dimmer::Dimmer(int maxBrightness, int minBrightness, long sunriseLength,
               long sunsetLength) {
  maxBright = maxBrightness;
  minBright = minBrightness;
  sunriseLen = sunriseLength;
  sunsetLen = sunsetLength;
}

void Dimmer::resetTiming() {
  startTime = myMillis();
  elapsedTime = 0UL;
  startBrightness = brightness;
}

void Dimmer::updateElapsedTime() {
  unsigned long currTime = myMillis();
  elapsedTime = (currTime - startTime);
}

void Dimmer::updateSunriseBrightness() {
  unsigned long brightnessBuffer =
      map(startBrightness, minBright, maxBright, 0, sunriseLen);
  brightness = min(maxBright, map(elapsedTime + brightnessBuffer, 0, sunriseLen,
                                  minBright, maxBright));
}

void Dimmer::updateSunsetBrightness() {
  unsigned long brightnessBuffer =
      map(startBrightness, maxBright, minBright, 0, sunsetLen);
  brightness = max(minBright, map(elapsedTime + brightnessBuffer, sunsetLen, 0,
                                  minBright, maxBright));
}

unsigned long Dimmer::myMillis() { return millis() + milliStart; }
