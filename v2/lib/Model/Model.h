#pragma once
#include <TimeLib.h>

class Model {
public:
  time_t currentTime;
  int brightness;
  long sunriseLength = 60;
  long sunsetLength = 60;
  time_t sunsetTime;
  time_t sunriseTime;
  int maxBrightnessPercent = 80;
  bool manualTiming = false;
  time_t manualSunriseTime;
  time_t manualSunsetTime;
};
