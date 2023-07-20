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
  startTime = currentTime;
  daylightSavingsTime = -1.0;
  latitude = lat;
  longitude = lon;
  timeZone = tmz;
  DACCHANNEL = dacChannel;
  setupMCP();
  setupSun(currentTime);
  updateSunriseTime();
  updateSunsetTime();
}

void Dimmer::clearDSTflag(){
  DSTChangeflag = 0;
}

int Dimmer::getDSTflag(){
  return DSTChangeflag;
}

double Dimmer::getDSTValue(){
  return daylightSavingsTime;
}

void Dimmer::run(Model &model) {
  updateState(model.currentTime);
  switch (state) {
  case Sunrise:
    if (model.manualTiming) {
      setManualSunsetSunriseTimes(model);
    } else {
      updateSunriseTime();
      model.sunriseTime =
          hourMinuteToTime(sunriseHour, sunriseMin, model.currentTime);
    }
    updateSunriseBrightness();
    break;
  case Sunset:
    if (model.manualTiming) {
      setManualSunsetSunriseTimes(model);
    } else {
      updateSunsetTime();
      model.sunsetTime =
          hourMinuteToTime(sunsetHour, sunsetMin, model.currentTime);
    }
    updateSunsetBrightness();
    break;
  default:
    break;
  }
  if (sunriseSeconds != (model.sunriseLength * 60)) {
    sunriseSeconds = model.sunriseLength * 60;
    if (state == Sunrise) {
      startBrightness = brightness;
      startTime = model.currentTime;
    }
  }
  if (sunsetSeconds != (model.sunsetLength * 60)) {
    sunsetSeconds = model.sunsetLength * 60;
    if (state == Sunset) {
      startBrightness = brightness;
      startTime = model.currentTime;
    }
  }
  model.brightnessPercent = ((float)brightness / (float)maxBright) * 100.0;
  scaleMaxBrightness(model.maxBrightnessPercent);
  setDacValue(brightness);
}

void Dimmer::scaleMaxBrightness(int percent) {
  scaledMaxBright = maxBright * (percent / 100);
}

void Dimmer::setModelSunriseSunsetTime(Model &model) {
  model.sunriseTime =
      hourMinuteToTime(sunriseHour, sunriseMin, model.currentTime);
  model.sunsetTime = hourMinuteToTime(sunsetHour, sunsetMin, model.currentTime);
}

void Dimmer::setupMCP() {
  if (!mcp.begin()) {
    Serial.println("Failed to find MCP4728 chip. Freezing");
    while (1) {
      delay(10);
    }
  }
}

void Dimmer::setupSun(time_t currentTime) {
  int m = month(currentTime);
  int d = day(currentTime);
  int dayOfWeek = weekday(currentTime);
  setDate(year(currentTime), m, d);
  updateDaylightSavings(m, d, dayOfWeek);
  sun.setPosition(latitude, longitude, timeZone + daylightSavingsTime);
}

void Dimmer::setDate(int year, int month, int day) {
  sun.setCurrentDate(year, month, day);
}

void Dimmer::setDacValue(int value) {
  value = constrain(value, minBright, scaledMaxBright);
  mcp.setChannelValue(DACCHANNEL, value, MCP4728_VREF_INTERNAL,
                      MCP4728_GAIN_2X);
}

void Dimmer::setManualSunsetSunriseTimes(Model &model) {
  sunriseHour = hour(model.manualSunriseTime);
  sunriseMin = minute(model.manualSunriseTime);
  sunriseMinPastMidnight = (sunriseHour * 60) + sunriseMin;
  sunsetHour = hour(model.manualSunsetTime);
  sunsetMin = minute(model.manualSunsetTime);
  sunsetMinPastMidnight = (sunsetHour * 60) + sunsetMin;
}

void Dimmer::updateDaylightSavings(int month, int day, int dayOfWeek){
  
  double tempDST = daylightSavingsTime;

  if(month > 11 || month < 3){
    daylightSavingsTime = 0.0;
  }else if(month > 3 && month < 11){
    daylightSavingsTime = 1.0;
  }else if(month == 3){
    if(day < 8){
      daylightSavingsTime = 0.0;
    }else if(day >= 8 && day < 14){
      if(day < (dayOfWeek + 7)){
        daylightSavingsTime = 0.0;
      }else if(day >= (dayOfWeek + 7)){
        daylightSavingsTime = 1.0;
      }
    }else if(day >= 14){
      daylightSavingsTime = 1.0;
    }
  }else if(month == 11){
    if(day > 7){
      daylightSavingsTime = 0.0;
    }else{
      if(day >= dayOfWeek){
        daylightSavingsTime = 0.0;
      }else if(day < dayOfWeek){
        daylightSavingsTime = 1.0;
      }
    }
  }

  //for changing the clock
  //initial setup shouldn't have to adjust the clock
  //so we check to see if this is our first time or not 
  //with -1 representing our first time setting DST value
  if((tempDST >= 0) && (tempDST != daylightSavingsTime)){
    DSTChangeflag = 1;
    sun.setTZOffset(timeZone + daylightSavingsTime);
  }else{
    sun.setTZOffset(timeZone + daylightSavingsTime);
  }
}

void Dimmer::updateState(time_t currentTime) {
  updateTotalElapsedSeconds(currentTime);
  int minutes = timeToMinPastMidnight(currentTime);
  if (minutes > sunsetMinPastMidnight) {
    if (state == Sunrise) {
      startTime = currentTime;
      startBrightness = brightness;
    }
    state = Sunset;
  } else if (minutes > sunriseMinPastMidnight) {
    if (state == Sunset) {
      startTime = currentTime;
      startBrightness = brightness;
    }
    state = Sunrise;
  } else {
    setDate(year(currentTime), month(currentTime), day(currentTime));
    updateDaylightSavings(month(currentTime), day(currentTime), weekday(currentTime));
  }
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

void Dimmer::updateSunriseBrightness() {
  unsigned long brightnessBuffer =
      map(startBrightness, minBright, maxBright, 0, sunriseSeconds);
  brightness = _min(maxBright, map(totalElapsedSeconds + brightnessBuffer, 0,
                                   sunriseSeconds, minBright, maxBright));
}

void Dimmer::updateSunsetTime() {
  sunsetMinPastMidnight = sun.calcSunset();
  sunsetHour = sunsetMinPastMidnight / 60;
  sunsetMin = sunsetMinPastMidnight % 60;
}

void Dimmer::updateTotalElapsedSeconds(time_t currentTime) {
  totalElapsedSeconds = currentTime - startTime;
}

int Dimmer::timeToMinPastMidnight(time_t t) {
  int minutes = hour(t) * 60;
  minutes += minute(t);
  return minutes;
}

time_t Dimmer::hourMinuteToTime(int hour, int minute, time_t now) {
  TimeElements tm = {0,          minute,          hour, weekday(now), day(now),
                     month(now), year(now) - 1970};
  return makeTime(tm);
}
