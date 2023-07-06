#pragma once
#include <SparkFun_RV8803.h>
#include <TimeLib.h>

class Dimmer {
public:
  int maxBright; // = 4095;
  int minBright; // = 500;
  long sunriseSeconds;
  long sunsetSeconds;

  int brightness;
  int startBrightness;
  unsigned long totalElapsedSeconds = 0UL;
  time_t startTime;

  Dimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
         long sunsetLenSeconds);
  TimeElements createTimeElements(RV8803 rtc);
  void setStartTime(TimeElements tm);
  void updateSunriseBrightness();
  void updateSunsetBrightness();

  unsigned long updateElapsedTime(TimeElements tm);
};
