#include <Adafruit_MCP4728.h>
#include <Arduino.h>
#include <Dimmer.h>
#include <Wire.h>
#include <ezButton.h>

#define CLOCKPIN 7

const int MAXBRIGHTNESS = 4095;
const int MINBRIGHTNESS = 500;
const long SUNRISELENGTH = 600000L;
const long SUNSETLENGTH = 600000L;

ezButton clock(CLOCKPIN);
Adafruit_MCP4728 mcp;
Dimmer dimmer(MAXBRIGHTNESS, MINBRIGHTNESS, SUNRISELENGTH, SUNSETLENGTH);

const MCP4728_channel_t DACCHANNEL = MCP4728_CHANNEL_A;

bool isClockOn();
void setupMCP();
void setDacValue(int value);

void setup() {
  Serial.begin(115200);
  clock.setDebounceTime(200);
  setupMCP();
}

void loop() {
  clock.loop();
  if (clock.isPressed() || clock.isReleased()) {
    dimmer.resetTiming();
  }
  if (isClockOn()) {
    dimmer.updateElapsedTime();
    dimmer.updateSunriseBrightness();
  } else {
    dimmer.updateElapsedTime();
    dimmer.updateSunsetBrightness();
  }
  setDacValue(dimmer.brightness);
  Serial.print("Brightness: ");
  Serial.println(dimmer.brightness);
  Serial.print("Elapsed: ");
  Serial.println(dimmer.elapsedTime);
  Serial.print("Timestamp: ");
  Serial.println(dimmer.myMillis());
}

bool isClockOn() {
  int state = clock.getState();
  return (state == LOW);
}

void setupMCP() {
  while (!Serial)
    delay(10);

  if (!mcp.begin()) {
    Serial.println("Failed to find MCP4728 chip");
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
