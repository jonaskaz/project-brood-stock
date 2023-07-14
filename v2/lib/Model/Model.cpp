#include "Model.h"

void Model::init(const char *ns, long defaultRiseLen, long defaultSetLen,
                 int defaultMaxBright, time_t defaultManRise,
                 time_t defaultManSet) {
  prefNS = ns;
  defaultSunriseLen = defaultRiseLen;
  defaultSunsetLen = defaultSetLen;
  defaultMaxBrightness = defaultMaxBright;
  defaultManualSunrise = (long)defaultManRise;
  defaultManualSunset = (long)defaultManSet;
  openPreferences();
  loadModel();
  closePreferences();
}

void Model::loadModel() {
  manualTiming = preferences.getBool(manuaTimingKey, false);
  long manSunrise = preferences.getLong(manualSunriseKey, defaultManualSunrise);
  manualSunriseTime = (time_t)manSunrise;
  long manSunset = preferences.getLong(manualSunsetKey, defaultManualSunset);
  manualSunsetTime = (time_t)manSunset;
  sunriseLength = preferences.getLong(sunriseLengthKey, defaultSunriseLen);
  sunsetLength = preferences.getLong(sunsetLengthKey, defaultSunsetLen);
  maxBrightnessPercent =
      preferences.getInt(maxBrightnessKey, defaultMaxBrightness);
}

void Model::saveModel() {
  openPreferences();
  preferences.putBool(manuaTimingKey, manualTiming);
  long manSunrise = (long)manualSunriseTime;
  preferences.putLong(manualSunriseKey, manSunrise);
  long manSunset = (long)manualSunsetTime;
  preferences.putLong(manualSunsetKey, manSunset);
  preferences.putLong(sunriseLengthKey, sunriseLength);
  preferences.putLong(sunsetLengthKey, sunsetLength);
  preferences.putInt(maxBrightnessKey, maxBrightnessPercent);
  closePreferences();
}

void Model::openPreferences() { preferences.begin(prefNS, false); };

void Model::closePreferences() { preferences.end(); };
