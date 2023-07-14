#include "Dimmer.h"
#include <Arduino.h>
#include <Adafruit_LiquidCrystal.h>
#include <TimeLib.h>

//KBIC Fish Hatchery lat, lon, and tz
#define LATITUDE 46.8421690
#define LONGITUDE -88.3803090
#define TIMEZONE_OFFSET -5.0

const int MAXBRIGHTNESS = 4095;
const int MINBRIGHTNESS = 300;
const long SUNRISELENGTHSECONDS = 3600L;
const long SUNSETLENGTHSECONDS = 3600L;
uint8_t prevLogMinute = 0;
TimeElements tm;

// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);
Dimmer dimmer; //declared here for scope

void setup() {
  Serial.begin(115200);
  uint8_t year = 2023;
  tm = {1, 1, 1, 1, 1, 1, year};
  //have to setup here for testing DAC to get feedback. before setup is too soon.
  dimmer.setupDimmer(MAXBRIGHTNESS, MINBRIGHTNESS, SUNRISELENGTHSECONDS,
              SUNSETLENGTHSECONDS, LATITUDE, LONGITUDE, TIMEZONE_OFFSET);
  dimmer.setDate(8, 27, 2023);
  dimmer.calcSunRise();
  dimmer.calcSunSet();
}

void loop() {

  Serial.print("Sunrise: ");
  Serial.print(dimmer.sunriseHour);
  Serial.print(":");
  Serial.print(dimmer.sunriseMin);
  Serial.print(" Sunset: ");
  Serial.print(dimmer.sunsetHour);
  Serial.print(":");
  Serial.println(dimmer.sunsetMin);

}
