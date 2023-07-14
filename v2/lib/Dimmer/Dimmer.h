#pragma once
#include <Adafruit_MCP4728.h>
#include <SparkFun_RV8803.h> //change to new clock later
#include <TimeLib.h>
#include <sunset.h>

class Dimmer {
private:
  MCP4728_channel_t DACCHANNEL;
  Adafruit_MCP4728 mcp;
  SunSet sun;
  double latitude;
  double longitude;
  double timeZone;

  void setupMCP();
  void setDacValue(int value);

public:
  int maxBright; // = 4095;
  int minBright; // = 500;
  long sunriseSeconds;
  long sunsetSeconds;

  int sunriseHour;
  int sunriseMin;
  int sunsetHour;
  int sunsetMin;

  int brightness;
  int startBrightness;
  unsigned long totalElapsedSeconds = 0UL;
  time_t startTime;

  Dimmer();
  Dimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
         long sunsetLenSeconds);
  Dimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
         long sunsetLenSeconds, double lat, double lon, double tmz);
  void setupDimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
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
