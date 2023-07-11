#pragma once
#include <TimeLib.h>

class Model {
public:
  time_t currentTime;
  int brightness;
  long sunriseLength;
  long sunsetLength;
  time_t sunsetTime;
  time_t sunriseTime;
  int maxBrightness;
  bool manualTiming;
};
