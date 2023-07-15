#include "Dimmer.h"
#include <Arduino.h>
#include <TimeLib.h>

Dimmer::Dimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
               long sunsetLenSeconds, double lat, double lon, double tmz,
               MCP4728_channel_t dacChannel) {
  maxBright = maxBrightness;
  minBright = minBrightness;
  sunriseSeconds = sunriseLenSeconds;
  sunsetSeconds = sunsetLenSeconds;
  brightness = minBright;
  startBrightness = brightness;
  latitude = lat;
  longitude = lon;
  timeZone = tmz;
  DACCHANNEL = dacChannel;
}

void Dimmer::init() {
  sun.setPosition(latitude, longitude, timeZone);
  setupMCP();
  updateSunriseTime();
  updateSunsetTime();
}

int Dimmer::timeToMinPastMidnight(time_t t) {
  int minutes = hour(t) * 60;
  minutes += minute(t);
  return minutes;
}

void Dimmer::updateState(time_t currentTime) {
  updateTotalElapsedSeconds(currentTime);
  int minutes = timeToMinPastMidnight(currentTime);
  if (minutes > sunriseMinPastMidnight) {
    state = Sunrise;
  } else {
    state = Sunset;
  }
}

void Dimmer::run(time_t currentTime) {
  updateState(currentTime);
  switch (state) {
  case Sunrise:
    updateSunsetTime();
    updateSunriseBrightness();
    setDacValue(brightness);
    break;
  case Sunset:
    updateSunriseTime();
    updateSunsetBrightness();
    setDacValue(brightness);
    break;
  }
}

void Dimmer::setupMCP() {
  while (!Serial)
    delay(10);
  if (!mcp.begin()) {
    Serial.println("Failed to find MCP4728 chip. Freezing");
    while (1) {
      delay(10);
    }
  }
}

void Dimmer::setDacValue(int value) {
  value = constrain(value, minBright, maxBright);
  mcp.setChannelValue(DACCHANNEL, value, MCP4728_VREF_INTERNAL,
                      MCP4728_GAIN_2X);
}

void Dimmer::setStartTime(TimeElements tm) { startTime = makeTime(tm); }

void Dimmer::updateTotalElapsedSeconds(time_t currentTime) {
  totalElapsedSeconds = currentTime - startTime;
}

void Dimmer::setDate(int year, int month, int day) {
  sun.setCurrentDate(year, month, day);
}

void Dimmer::updateSunriseBrightness() {
  unsigned long brightnessBuffer =
      map(startBrightness, minBright, maxBright, 0, sunriseSeconds);
  brightness = _min(maxBright, map(totalElapsedSeconds + brightnessBuffer, 0,
                                   sunriseSeconds, minBright, maxBright));
  setDacValue(brightness);
}

void Dimmer::updateSunsetBrightness() {
  unsigned long brightnessBuffer =
      map(startBrightness, maxBright, minBright, 0, sunsetSeconds);
  brightness = _max(minBright, map(totalElapsedSeconds + brightnessBuffer,
                                   sunsetSeconds, 0, minBright, maxBright));
  setDacValue(brightness);
}

void Dimmer::setLatLon(double lat, double lon) {
  latitude = lat;
  longitude = lon;
  sun.setPosition(latitude, longitude, timeZone);
}

void Dimmer::updateSunriseTime() {
  sunriseMinPastMidnight = sun.calcSunrise();
  sunriseHour = sunriseMinPastMidnight / 60;
  sunriseMin = sunriseMinPastMidnight % 60;
}

void Dimmer::updateSunsetTime() {
  sunsetMinPastMidnight = sun.calcSunset();
  sunsetHour = sunsetMinPastMidnight / 60;
  sunsetMin = sunsetMinPastMidnight % 60;
}

void Dimmer::setTimeZone(double tmz) {
  timeZone = tmz;
  sun.setTZOffset(timeZone);
}

double Dimmer::getTimeZone() { return timeZone; }
