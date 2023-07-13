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
  setupMCP();
}

Dimmer::Dimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
               long sunsetLenSeconds, double lat, double lon, double tmz) {
  maxBright = maxBrightness;
  minBright = minBrightness;
  sunriseSeconds = sunriseLenSeconds;
  sunsetSeconds = sunsetLenSeconds;
  brightness = minBright;
  startBrightness = brightness;
  latitude = lat;
  longitude = lon;
  timeZone = tmz;
  sun.setPosition(latitude, longitude, timeZone);
  setupMCP();
}

TimeElements Dimmer::createTimeElements(RV8803 rtc) {
  TimeElements tm = {rtc.getSeconds(),     rtc.getMinutes(), rtc.getHours(),
                     rtc.getWeekday() + 1, rtc.getDate(),    rtc.getMonth(),
                     rtc.getYear()};
  return tm;
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

unsigned long Dimmer::updateElapsedTime(TimeElements tm) {
  time_t currentTime = makeTime(tm);
  totalElapsedSeconds = currentTime - startTime;
  return totalElapsedSeconds;
}

void Dimmer::setDate(int year, int month, int day){
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

void Dimmer::setLatLon(double lat, double lon){
  latitude = lat;
  longitude = lon;
  sun.setPosition(latitude, longitude, timeZone);
}

//calculates the sunrise's hour and mins as separate variables
void Dimmer::calcSunRise(){
  int sunrise = sun.calcSunrise();
  sunriseHour = sunrise/60;
  sunriseMin = sunrise%60;
}

//calculates the sunset's hour and mins as separate variables
void Dimmer::calcSunSet(){
  int sunset = sun.calcSunset();
  sunsetHour = sunset/60;
  sunsetMin = sunset%60;
}

void Dimmer::setTimeZone(double tmz){
  timeZone = tmz;
  sun.setTZOffset(timeZone);
}

double Dimmer::getLatitude(){
  return latitude;
}

double Dimmer::getLongitude(){
  return longitude;
}

double Dimmer::getTimeZone(){
  return timeZone;
}
