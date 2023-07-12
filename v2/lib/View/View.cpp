#include "View.h"
#include "Model.h"
#include <Adafruit_LiquidCrystal.h>
#include <Arduino.h>
#include <TimeLib.h>
#include <Wire.h>

void View::init() {
  if (!lcd.begin(16, 2)) {
    Serial.println("Could not init lcd. Check wiring.");
    while (1)
      ;
  }
  lcd.setBacklight(HIGH);
}

void View::showScreen(Model model) {
  switch (currentScreen) {
  case home:
    showHome(model.sunriseTime, model.sunsetTime);
    break;
  case manualMode:
    showManualMode(model.manualTiming);
    break;
  case sunriseLength:
    showSunriseLength(model.sunriseLength);
    break;
  case sunsetLength:
    showSunsetLength(model.sunsetLength);
    break;
  case maxBrightnessPercent:
    showMaxBrightPercent(model.maxBrightnessPercent);
    break;
  case manualSunrise:
    showManualSunrise(model.manualSunriseTime);
    break;
  case manualSunset:
    showManualSunset(model.manualSunsetTime);
    break;
  default:
    break;
  }
  if (editMode) {
    lcd.setCursor(0, 1);
    lcd.blink();
  } else {
    lcd.noBlink();
  }
}

void View::nextScreen() {
  if (currentScreen == manualSunset) {
    currentScreen = home;
  } else {
    int currentScreenInt = (int)currentScreen;
    currentScreenInt++;
    currentScreen = (Screen)currentScreenInt;
  }
}

String View::timeToString(time_t t) {
  int h = hour(t);
  int m = minute(t);
  String hString;
  String mString;
  String timeString;
  hString = zeroPadTimeString(String(h));
  mString = zeroPadTimeString(String(m));
  timeString = hString + ":" + mString;
  return timeString;
}

String View::zeroPadTimeString(String t) {
  if (t.length() == 1) {
    t = "0" + t;
  }
  return t;
}

void View::showHome(time_t sunrise, time_t sunset) {
  lcd.setCursor(0, 0);
  lcd.print("Sunrise: ");
  String sunriseString = timeToString(sunrise);
  lcd.print(sunriseString);
  lcd.setCursor(0, 1);
  lcd.print("Sunset: ");
  String sunsetString = timeToString(sunset);
  lcd.print(sunsetString);
}

void View::showManualMode(bool manual) {
  lcd.setCursor(0, 0);
  lcd.print("Manual Timing:");
  lcd.setCursor(0, 1);
  if (manual) {
    lcd.print("On");
  } else {
    lcd.print("Off");
  }
}

void View::showSunriseLength(long sunriseLength) {
  lcd.setCursor(0, 0);
  lcd.print("Sunrise Length:");
  lcd.setCursor(0, 1);
  lcd.print(sunriseLength);
}

void View::showSunsetLength(long sunsetLength) {
  lcd.setCursor(0, 0);
  lcd.print("Sunset Length:");
  lcd.setCursor(0, 1);
  lcd.print(sunriseLength);
}

void View::showMaxBrightPercent(int maxBrightnessPercent) {
  lcd.setCursor(0, 0);
  lcd.print("Max Brightness:");
  lcd.setCursor(0, 1);
  lcd.print(maxBrightnessPercent);
  lcd.print("%");
}

void View::showManualSunrise(time_t sunriseTime) {
  lcd.setCursor(0, 0);
  lcd.print("Manual Sunrise:");
  lcd.setCursor(0, 1);
  String sunriseTimeStr = timeToString(sunriseTime);
  lcd.print(sunriseTimeStr);
}

void View::showManualSunset(time_t sunsetTime) {
  lcd.setCursor(0, 0);
  lcd.print("Manual Sunset:");
  lcd.setCursor(0, 1);
  String sunsetTimeStr = timeToString(sunsetTime);
  lcd.print(sunsetTimeStr);
}
