#include <Adafruit_MCP4728.h>
#include <Arduino.h>
#include <Adafruit_BusIO_Register.h>
#include <Wire.h>
#include <ezButton.h>

#define CLOCKPIN 7

ezButton clock(CLOCKPIN);
Adafruit_MCP4728 mcp;

const int LED = 5;
//const int MAXBRIGHTNESS = 255.0;
const MCP4728_channel_t DACCHANNEL = MCP4728_CHANNEL_A;
const int MAXBRIGHTNESS = 4095;
const int MINBRIGHTNESS = 0;
const int INCREMENT = 10;
const int LOOPDELAY = 30;
int brightness = 0;
double elapsedTime = 0;
unsigned long prevTime;
unsigned long rampTime;

bool isClockOn();
void setupMCP();
void setDacValue(int value);
unsigned long dimmerControl(bool);

void setup() {
  Serial.begin(115200);
  clock.setDebounceTime(200);
  setupMCP();
  prevTime = millis();
  rampTime = 60000;//1 minute change time here
}

void loop() {

  clock.loop();
  if (isClockOn()) {
    if (brightness < MAXBRIGHTNESS) {
      prevTime = dimmerControl(true);
    }
  } else {
    if (brightness > MINBRIGHTNESS) {
      prevTime = dimmerControl(false);
    }
  }
  setDacValue(brightness);
  Serial.println(brightness);
  //delay(LOOPDELAY);
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

//ramps up the brightness of the light
//also written to handle millis() overflow
unsigned long dimmerControl(bool increase) {

  unsigned long currTime = millis();
	//unsigned long should account for overflow on arithmetic
	elapsedTime += (currTime - prevTime);
  double ratio;
  //dim up or down based on what is needed
  if(increase){
    ratio = elapsedTime/rampTime;
    if(ratio > 1){
      ratio = 1;
    }
  }else{
    ratio = 1 - (elapsedTime/rampTime);
    if(ratio < 0){
      ratio = 0;
    }
  }
  brightness = round(MAXBRIGHTNESS * ratio);
    
  return currTime;
}
