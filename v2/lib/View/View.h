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
  maxBrightness = 4,
  manualSunrise = 5,
  manualSunset = 6
};

class View {
public:
  View() : lcd(0){};
  Adafruit_LiquidCrystal lcd;
  void init();
  void showScreen(Screen currentScreen, Model model);

private:
  String timeToString(time_t t);
  void showHome(time_t sunrise, time_t sunset);
  String zeroPadTimeString(String t);
};
