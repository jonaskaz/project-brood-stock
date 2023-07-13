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
  sunriseLength = preferences.getLong(sunriseLengthKey, defaultSunriseLen);
  sunsetLength = preferences.getLong(sunsetLengthKey, defaultSunsetLen);
  maxBrightnessPercent =
      preferences.getInt(maxBrightnessKey, defaultMaxBrightness);
  manualSunriseTime =
      (time_t)preferences.getLong(manualSunriseKey, defaultManualSunrise);
  manualSunsetTime =
      (time_t)preferences.getLong(manualSunsetKey, defaultManualSunset);
}

void Model::saveModel() {
  openPreferences();
  Serial.println(manualSunriseTime);
  Serial.println(manualSunsetTime);
  long manualRiseLong = (long)manualSunriseTime;
  long manualSetLong = (long)manualSunsetTime;
  Serial.println(manualRiseLong);
  Serial.println(manualSetLong);
  preferences.putLong(sunriseLengthKey, sunriseLength);
  preferences.putLong(sunsetLengthKey, sunsetLength);
  preferences.putInt(maxBrightnessKey, maxBrightnessPercent);
  preferences.putLong(manualSunriseKey, (long)manualRiseLong);
  preferences.putLong(manualSunsetKey, (long)manualSetLong);
  closePreferences();
}

void Model::openPreferences() { preferences.begin(prefNS, false); };

void Model::closePreferences() { preferences.end(); };
