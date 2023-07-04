#pragma once

class Dimmer {
public:
  int maxBright; // = 4095;
  int minBright; // = 500;
  long sunriseSeconds;
  long sunsetSeconds;

  int brightness = minBright;
  int startBrightness = brightness;
  unsigned long totalElapsedSeconds = 0UL;
  int startHour = 0;
  int startMinute = 0;
  int startSecond = 0;

  Dimmer(int maxBrightness, int minBrightness, long sunriseLenSeconds,
         long sunsetLenSeconds);

  void setStartTime(int hour, int minute, int second);
  void updateSunriseBrightness();
  void updateSunsetBrightness();

  /**
   * Calculate the amount of time elapsed since startHour, startMinute, and
   * startSecond
   *
   * @param currentHour The current hour
   * @param currentMinute The current minute
   * @param currentSeconds The current seconds
   * @return totalElapsedSeconds
   *
   */
  unsigned long updateElapsedTime(int currentHour, int currentMinute,
                                  int currentSecond);
};
