#pragma once
#include <Preferences.h>
#include <TimeLib.h>

class Model {
public:
  void init(const char *ns, long defaultRiseLen, long defaultSetLen,
            int defaultMaxBright, time_t defaultManRise, time_t defaultManSet);
  Preferences preferences;
  void loadModel();
  void saveModel();
  void openPreferences();
  void closePreferences();

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

private:
  const char *prefNS;
  long defaultSunriseLen;
  long defaultSunsetLen;
  int defaultMaxBrightness;
  long defaultManualSunrise;
  long defaultManualSunset;
  const char *sunriseLengthKey = "sunriseLength";
  const char *sunsetLengthKey = "sunsetLength";
  const char *maxBrightnessKey = "maxBrightness";
  const char *manualSunriseKey = "manualSunriseTime";
  const char *manualSunsetKey = "manualSunsetTime";
};
