#include <Adafruit_MCP4728.h>
#include <Arduino.h>
#include <Wire.h>
#include <ezButton.h>

#define CLOCKPIN 7

ezButton clock(CLOCKPIN);
Adafruit_MCP4728 mcp;

const MCP4728_channel_t DACCHANNEL = MCP4728_CHANNEL_A;
const int MAXBRIGHTNESS = 4095;
const int MINBRIGHTNESS = 500;
const int INCREMENT = 10;
const int LOOPDELAY = 30;
const long RAMPTIME = 10000;

int brightness = MINBRIGHTNESS;
int startBrightness = brightness;
unsigned long elapsedTime = 0;
unsigned long startTime;

bool isClockOn();
void setupMCP();
void setDacValue(int value);
void updateSunriseBrightness();
void updateSunsetBrightness();
void updateElapsedTime();

void setup() {
  Serial.begin(115200);
  clock.setDebounceTime(200);
  setupMCP();
  startTime = millis();
  Serial.println(RAMPTIME);
}

void loop() {
  clock.loop();
  if (clock.isPressed() || clock.isReleased()) {
    startTime = millis();
    elapsedTime = 0;
    startBrightness = brightness;
  }
  int prevBrightness = brightness;
  if (isClockOn()) {
    updateElapsedTime();
    updateSunriseBrightness();
  } else {
    updateElapsedTime();
    updateSunsetBrightness();
  }
  if (prevBrightness != brightness) {
    setDacValue(brightness);
  }
  Serial.print("Brightness: ");
  Serial.println(brightness);
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

void updateElapsedTime() {
  unsigned long currTime = millis();
  elapsedTime = (currTime - startTime);
}

void updateSunriseBrightness() {
  int brightnessBuffer =
      map(startBrightness, MINBRIGHTNESS, MAXBRIGHTNESS, 0, RAMPTIME);
  brightness = min(MAXBRIGHTNESS, map(elapsedTime + brightnessBuffer, 0,
                                      RAMPTIME, MINBRIGHTNESS, MAXBRIGHTNESS));
}

void updateSunsetBrightness() {
  int brightnessBuffer =
      map(startBrightness, MAXBRIGHTNESS, MINBRIGHTNESS, 0, RAMPTIME);
  brightness = max(MINBRIGHTNESS, map(elapsedTime + brightnessBuffer, RAMPTIME,
                                      0, MINBRIGHTNESS, MAXBRIGHTNESS));
}
