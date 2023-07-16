#include "Dimmer.h"
#include <Arduino.h>
#include <TimeLib.h>

void Dimmer::init(int maxBrightness, int minBrightness, long sunriseLenSeconds,
                  long sunsetLenSeconds, double lat, double lon, double tmz,
                  MCP4728_channel_t dacChannel, time_t currentTime) {
  maxBright = maxBrightness;
  minBright = minBrightness;
  sunriseSeconds = sunriseLenSeconds;
  sunsetSeconds = sunsetLenSeconds;
  brightness = minBright;
  startBrightness = minBright;
  latitude = lat;
  longitude = lon;
  timeZone = tmz;
  DACCHANNEL = dacChannel;
  setupMCP();
  setupSun(currentTime, tmz);
  updateSunriseTime();
  updateSunsetTime();
}

void Dimmer::run(Model &model) {
  updateState(model.currentTime);
  switch (state) {
  case Sunrise:
    updateSunsetTime();
    model.sunsetTime =
        hourMinuteToTime(sunsetHour, sunsetMin, model.currentTime);
    updateSunriseBrightness();
    break;
  case Sunset:
    updateSunriseTime();
    model.sunriseTime =
        hourMinuteToTime(sunriseHour, sunriseMin, model.currentTime);
    updateSunsetBrightness();
    break;
  default:
    break;
  }
  model.brightnessPercent = (brightness / maxBright) * 100;
  scaleMaxBrightness(model.maxBrightnessPercent);
  setDacValue(brightness);
}

void Dimmer::setupSun(time_t currentTime, double tmz) {
  sun.setTZOffset(tmz);
  setDate(year(currentTime), month(currentTime), day(currentTime));
  sun.setPosition(latitude, longitude, timeZone);
}

int Dimmer::timeToMinPastMidnight(time_t t) {
  int minutes = hour(t) * 60;
  minutes += minute(t);
  return minutes;
}

time_t hourMinuteToTime(int hour, int minute, time_t now) {
  TimeElements tm = {0,          minute,          hour, weekday(now), day(now),
                     month(now), year(now) - 1970};
  return makeTime(tm);
}

void Dimmer::updateState(time_t currentTime) {
  updateTotalElapsedSeconds(currentTime);
  int minutes = timeToMinPastMidnight(currentTime);
  if (minutes > sunriseMinPastMidnight) {
    if (state == Sunset) {
      startTime = currentTime;
      startBrightness = brightness;
    }
    state = Sunrise;
  } else {
    if (state == Sunrise) {
      startTime = currentTime;
      startBrightness = brightness;
    }
    state = Sunset;
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
  value = constrain(value, minBright, scaledMaxBright);
  mcp.setChannelValue(DACCHANNEL, value, MCP4728_VREF_INTERNAL,
                      MCP4728_GAIN_2X);
}

void Dimmer::updateTotalElapsedSeconds(time_t currentTime) {
  totalElapsedSeconds = currentTime - startTime;
}

void Dimmer::setDate(int year, int month, int day) {
  sun.setCurrentDate(year, month, day);
}

void Dimmer::setModelSunriseSunsetTime(Model &model) {
  model.sunriseTime =
      hourMinuteToTime(sunriseHour, sunriseMin, model.currentTime);
  model.sunsetTime = hourMinuteToTime(sunsetHour, sunsetMin, model.currentTime);
}

void Dimmer::scaleMaxBrightness(int percent) {
  scaledMaxBright = maxBright * (percent / 100);
}

void Dimmer::updateSunriseBrightness() {
  unsigned long brightnessBuffer =
      map(startBrightness, minBright, maxBright, 0, sunriseSeconds);
  brightness = _min(maxBright, map(totalElapsedSeconds + brightnessBuffer, 0,
                                   sunriseSeconds, minBright, maxBright));
}

void Dimmer::updateSunsetBrightness() {
  unsigned long brightnessBuffer =
      map(startBrightness, maxBright, minBright, 0, sunsetSeconds);
  brightness = _max(minBright, map(totalElapsedSeconds + brightnessBuffer,
                                   sunsetSeconds, 0, minBright, maxBright));
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
