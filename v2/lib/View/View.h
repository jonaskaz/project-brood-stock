#pragma once
#include "Model.h"
#include <Adafruit_LiquidCrystal.h>
#include <Arduino.h>
#include <TimeLib.h>

enum Screen {
  home = 0,
  currentTime = 1,
  manualMode = 2,
  manualSunrise = 3,
  manualSunset = 4,
  sunriseLength = 5,
  sunsetLength = 6,
  maxBrightnessPercent = 7
};

class View {
public:
  /**
   * Default constructor for View and lcd. Initializes lcd on default i2c
   * address.
   */
  View() : lcd(0){};
  Adafruit_LiquidCrystal lcd;
  /**
   * Starts lcd and turns on backlight
   */
  void init();
  Screen currentScreen = home;
  bool editMode = false;
  /**
   * Show the currentScreen with model data on the lcd. Places curser in
   * blinking mode if editing is on
   *
   * @param model with all relevant data
   */
  void showScreen(Model model);
  /**
   * Update currentScreen to show the next screen according to Screen enum
   */
  void nextScreen();

private:
  /**
   * Converts a unix time_t timestamp into human readable String. The string
   * includes time only in the format hour:minute. Hour and minute are zero
   * padded up to length 2
   *
   * @param t unix timestamp
   * @return time string EX: 04:33
   */
  String timeToHourMinuteString(time_t t);
  /**
   * Converts a unix time_t timestamp into human readable String. The string
   * shows date and time.
   *
   * @param t unix timestamp
   * @return datetime string EX: 05/22/2023 04:33
   */
  String timeToDateTimeString(time_t t);
  /**
   * Pads string t at the beginning with zeros up to length 2.
   *
   * @param t string to pad
   * @return string t with 0 added if length is 1
   */
  String zeroPadTimeString(String t);
  void showHome(time_t sunrise, time_t sunset);
  void showCurrentTime(time_t currentTime);
  void showManualMode(bool manual);
  void showManualSunrise(time_t sunriseTime);
  void showManualSunset(time_t sunsetTime);
  void showSunriseLength(long sunriseLength);
  void showSunsetLength(long sunsetLength);
  void showMaxBrightPercent(int maxBrightnessPercent);
};
