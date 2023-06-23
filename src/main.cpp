#include <Adafruit_MCP4728.h>
#include <Arduino.h>
#include <Wire.h>
#include <ezButton.h>

#define CLOCKPIN 7

ezButton clock(CLOCKPIN);
Adafruit_MCP4728 mcp;

const MCP4728_channel_t DACCHANNEL = MCP4728_CHANNEL_A;
const int MAXBRIGHTNESS = 4095;
const int MINBRIGHTNESS = 0;
const int INCREMENT = 10;
const int LOOPDELAY = 30;
int brightness = 0;

bool isClockOn();
void setupMCP();
void setDacValue(int value);
void dimOn(int seconds);

void setup() {
  Serial.begin(115200);
  clock.setDebounceTime(200);
  setupMCP();
}

void loop() {
  clock.loop();
  if (isClockOn()) {
    if (brightness < MAXBRIGHTNESS) {
      brightness += INCREMENT;
    }
  } else {
    if (brightness > MINBRIGHTNESS) {
      brightness -= INCREMENT;
    }
  }
  setDacValue(brightness);
  Serial.println(brightness);
  delay(LOOPDELAY);
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
  value = min(MAXBRIGHTNESS, value);
  value = max(MINBRIGHTNESS, value);
  mcp.setChannelValue(DACCHANNEL, value, MCP4728_VREF_INTERNAL,
                      MCP4728_GAIN_2X);
}
