#pragma once
#include <SparkFun_RV8803.h>
#include <TimeLib.h>
#include <sunset.h>

class Dimmer {
private:
  double latitude;
  double longitude;
  double timeZone;

public:
  int maxBright; // = 4095;
  int minBright; // = 500;
  long sunriseSeconds;
  long sunsetSeconds;
  
  SunSet sun;
  int sunriseHour;
  int sunriseMin;
  int sunsetHour;
  int sunsetMin;

  int brightness;
  int startBrightness;
  unsigned long totalElapsedSeconds = 0UL;
  time_t startTime;

  Dimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
         long sunsetLenSeconds);
  Dimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
         long sunsetLenSeconds, double lat, double lon, double tmz);
  TimeElements createTimeElements(RV8803 rtc);
  void setStartTime(TimeElements tm);
  void setLatLon(double, double);
  void setTimeZone(double);
  void setDate(int, int, int);
  double getLatitude();
  double getLongitude();
  double getTimeZone();
  void calcSunSet();
  void calcSunRise();
  void updateSunriseBrightness();
  void updateSunsetBrightness();

  unsigned long updateElapsedTime(TimeElements tm);
};
