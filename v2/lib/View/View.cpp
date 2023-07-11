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

void View::showScreen(Screen currentScreen, Model model) {
  lcd.clear();
  lcd.noBlink();
  switch (currentScreen) {
  case home:
    showHome(model.sunriseTime, model.sunsetTime);
    break;
  case manualMode:
    showManualMode(model.manualTiming);
    break;
  case sunriseLength:
    break;
  case sunsetLength:
    break;
  case maxBrightness:
    break;
  case manualSunrise:
    break;
  case manualSunset:
    break;
  default:
    break;
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
