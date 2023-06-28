#pragma once

class Dimmer {
public:
  int maxBright;   // = 4095;
  int minBright;   // = 500;
  long sunriseLen; // = 600000L; // 10 minutes
  long sunsetLen;

  int brightness = minBright;
  int startBrightness = brightness;
  unsigned long elapsedTime = 0UL;
  unsigned long startTime = myMillis();
  unsigned long milliStart = 0UL;

  // Constructor
  Dimmer(int maxBrightness, int minBrightness, long riseTime, long setTime);

  void resetTiming();
  void updateSunriseBrightness();
  void updateSunsetBrightness();
  void updateElapsedTime();
  unsigned long myMillis();
};
