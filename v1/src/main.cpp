#include <Adafruit_MCP4728.h>
#include <Arduino.h>
#include <Dimmer.h>
#include <SparkFun_RV8803.h>
#include <Wire.h>
#include <ezButton.h>

#define CLOCKPIN 7

const int MAXBRIGHTNESS = 4095;
const int MINBRIGHTNESS = 500;
const long SUNRISELENGTH = 100000L;
const long SUNSETLENGTH = 100000L;
uint8_t prevLogMinute = 0;
const MCP4728_channel_t DACCHANNEL = MCP4728_CHANNEL_A;

ezButton clock(CLOCKPIN);
RV8803 rtc;
Adafruit_MCP4728 mcp;
Dimmer dimmer(MAXBRIGHTNESS, MINBRIGHTNESS, SUNRISELENGTH, SUNSETLENGTH);

bool isClockOn();
void setupMCP();
void setDacValue(int value);
void logInfo(uint8_t hour, uint8_t minute, uint8_t second, int brightness,
             unsigned long totalElapsedSeconds, bool isClockOn);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  clock.setDebounceTime(200);
  setupMCP();
  if (rtc.begin() == false) {
    Serial.println("Device not found. Please check wiring. Freezing.");
    while (1)
      ;
  }
}

void loop() {
  clock.loop();
  rtc.updateTime();
  uint8_t currHour = rtc.getHours();
  uint8_t currMinute = rtc.getMinutes();
  uint8_t currSecond = rtc.getSeconds();
  if (clock.isPressed() || clock.isReleased()) {
    dimmer.setStartTime(currHour, currMinute, currSecond);
    dimmer.startBrightness = dimmer.brightness;
    dimmer.totalElapsedSeconds = 0UL;
  }
  if (isClockOn()) {
    dimmer.updateElapsedTime(currHour, currMinute, currSecond);
    dimmer.updateSunriseBrightness();
  } else {
    dimmer.updateElapsedTime(currHour, currMinute, currSecond);
    dimmer.updateSunsetBrightness();
  }
  setDacValue(dimmer.brightness);

  // Log info once a minute
  if (prevLogMinute != currMinute) {
    prevLogMinute = currMinute;
    logInfo(currHour, currMinute, currSecond, dimmer.brightness,
            dimmer.totalElapsedSeconds, isClockOn());
  }
}

bool isClockOn() {
  int state = clock.getState();
  return (state == LOW);
}

void setupMCP() {
  while (!Serial)
    delay(10);
  if (!mcp.begin()) {
    Serial.println("Failed to find MCP4728 chip. Freezing");
    while (1) {
      delay(10);
    }
  }
}

void setDacValue(int value) {
  value = constrain(value, MINBRIGHTNESS, MAXBRIGHTNESS);
  mcp.setChannelValue(DACCHANNEL, value, MCP4728_VREF_INTERNAL,
                      MCP4728_GAIN_2X);
}

/**
 * Log a row of info in the following format:
 *
 * hour,minute,second,brightness,totalElapsedSeconds,isClockOn
 *
 */
void logInfo(uint8_t hour, uint8_t minute, uint8_t second, int brightness,
             unsigned long totalElapsedSeconds, bool isClockOn) {
  Serial.print(hour);
  Serial.print(",");
  Serial.print(minute);
  Serial.print(",");
  Serial.print(second);
  Serial.print(",");
  Serial.print(brightness);
  Serial.print(",");
  Serial.print(totalElapsedSeconds);
  Serial.print(",");
  Serial.println(isClockOn);
}
