#pragma once
#include "Model.h"
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
  /**
   * Intializes member vars. Sets up MCP and sun library. Calculates intial
   * sunset and sunrise time.
   *
   * @param maxBrightness the max DAC brightness value
   * @param minBrightness the min DAC brightness value
   * @param sunriseLenSeconds the length of sunrise in seconds
   * @param sunsetLenSeconds the length of sunset in seconds
   * @param dacChannel the DAC channel to write to
   * @param currentTime the current time in unix format
   */
  void init(int maxBrightness, int minBrightness, long sunriseLenSeconds,
            long sunsetLenSeconds, double lat, double lon, double tmz,
            MCP4728_channel_t dacChannel, time_t currentTime);
  /**
   * Updates dimmer state and then the brightness based on elapsed time and
   * sends the value to the DAC. Updates the model with the latest sunrise and
   * sunset times as well as brightness.
   *
   * @param model The data model
   */
  void run(Model &model);
  /**
   * Scale the scaledMaxBright variable based on the given percentage. Allows
   * user to adjust max brightness.
   *
   * @param percent percent to scale max brightness
   */
  void scaleMaxBrightness(int percent);
  /**
   * Based on the current sunrise and sunset times, calculate a unix timestamp
   * and update the model.
   *
   * @param model the data model
   */
  void setModelSunriseSunsetTime(Model &model);

private:
  int sunriseMinPastMidnight;
  int sunsetMinPastMidnight;
  int maxBright;
  int scaledMaxBright;
  int minBright;
  long sunriseSeconds;
  long sunsetSeconds;

  int sunriseHour;
  int sunriseMin;
  int sunsetHour;
  int sunsetMin;

  void setupMCP();
  void setupSun(time_t currentTime, double tmz);
  void setDate(int year, int month, int day);
  void setDacValue(int value);
  /**
   * Update the state of the dimmer based on the current time. If a new state
   * change occurs, reset the startBrightness and startTime
   *
   * @param currentTime the time now in unix format
   */
  void updateState(time_t currentTime);
  /**
   * Calculate the sunset time using the sun library based on lat, lon, date,
   * and timezone
   */
  void updateSunsetTime();
  /**
   * Calculate the sunrise time using the sun library based on lat, lon, date,
   * and timezone
   */
  void updateSunriseTime();
  /**
   * Update the brightness member based on the amount of time elapsed since
   * startTime, and the desired scaledMaxBrightness. Uses linear brightness
   * increase over time. Prevents jumps in brightness if a state change occurs
   * before max or min brightness was reached. Prevents brightness from going
   * above scaledMaxBrightness
   */
  void updateSunriseBrightness();
  /**
   * Update the brightness member based on the amount of time elapsed since
   * startTime and the desired minBright. Uses linear brightness decrease over
   * time. Prevents jumps in brightness if a state change occurs before max or
   * min brightness was reached. Prevents brightness from going below minBright.
   */
  void updateSunsetBrightness();
  /**
   * Update the totalElapsedSeconds variable by subtracting current time and the
   * start time of the currrent state.
   */
  void updateTotalElapsedSeconds(time_t currentTime);
  /**
   * Convert unix time to minutes elapsed since midnight.
   *
   * @return int minutes since midnight
   */
  int timeToMinPastMidnight(time_t t);
  /**
   * Convert and hour and minute into a unix timestamp based on the current
   * date.
   *
   * @param hour the hour to convert
   * @param the minute to convert
   * @param now the current time in unix format
   *
   * @return the hour and minute today in unix format
   */
  time_t hourMinuteToTime(int hour, int minute, time_t now);
};
