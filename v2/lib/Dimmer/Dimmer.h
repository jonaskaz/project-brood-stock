#pragma once
#include <Adafruit_MCP4728.h>
#include <TimeLib.h>
#include <sunset.h>

class Dimmer {
public:
  enum State { Sunrise = 0, Sunset = 1 };
  State state;
  MCP4728_channel_t DACCHANNEL;
  Adafruit_MCP4728 mcp;
  SunSet sun;
  double latitude;
  double longitude;
  double timeZone;

  int brightness;
  int startBrightness;
  unsigned long totalElapsedSeconds = 0UL;
  time_t startTime;

  Dimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
         long sunsetLenSeconds, double lat, double lon, double tmz,
         MCP4728_channel_t dacChannel);

  void updateState(time_t currentTime);

  /**
   * Updates the brightness based on elapsed time and sends the value to the
   * DAC.
   *
   */
  void run(time_t currentTime);
  void init();
  void setupMCP();
  void setDacValue(int value);

  double getTimeZone();

  void setStartTime(TimeElements tm);
  void setLat(double lat);
  void setLon(double lon);
  void setTimeZone(double);
  void setDate(int, int, int);

private:
  int sunriseMinPastMidnight;
  int sunsetMinPastMidnight;
  int maxBright; // = 4095;
  int minBright; // = 500;
  long sunriseSeconds;
  long sunsetSeconds;

  int sunriseHour;
  int sunriseMin;
  int sunsetHour;
  int sunsetMin;

  void updateSunsetTime();
  void updateSunriseTime();
  void updateSunriseBrightness();
  void updateSunsetBrightness();

  void updateTotalElapsedSeconds(time_t currentTime);
  int timeToMinPastMidnight(time_t t);
};
