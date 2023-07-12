#pragma once
#include "Model.h"
#include <Adafruit_LiquidCrystal.h>
#include <Arduino.h>
#include <TimeLib.h>

enum Screen {
  home = 0,
  manualMode = 1,
  sunriseLength = 2,
  sunsetLength = 3,
  maxBrightnessPercent = 4,
  manualSunrise = 5,
  manualSunset = 6
};

class View {
public:
  View() : lcd(0){};
  Adafruit_LiquidCrystal lcd;
  void init();
  Screen currentScreen = home;
  bool editMode = false;
  void showScreen(Model model);
  void nextScreen();

private:
  String timeToString(time_t t);
  String zeroPadTimeString(String t);
  void showHome(time_t sunrise, time_t sunset);
  void showManualMode(bool manual);
  void showSunriseLength(long sunriseLength);
  void showSunsetLength(long sunsetLength);
  void showMaxBrightPercent(int maxBrightnessPercent);
  void showManualSunrise(time_t sunriseTime);
  void showManualSunset(time_t sunsetTime);
};
