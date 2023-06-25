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
unsigned long time;

bool isClockOn();
void setupMCP();
void setDacValue(int value);
void dimmerControl(double, bool);

void setup() {
  Serial.begin(115200);
  clock.setDebounceTime(200);
  setupMCP();
  time = 60000;//1 minute
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
//ramps up the brightness of the light
//also written to handle millis() overflow, I hope
void dimmerControl(double rampTime, bool increase) {

  unsigned long currTime = millis();
  unsigned long prevTime = currTime;
  double elapsedTime = 0;

  while(elapsedTime < rampTime) {
    currTime = millis();
	//unsigned long should account for overflow on arithmetic
	elapsedTime += (currTime - prevTime);
    double ratio;
    //dim up or down based on what is needed
    if(increase) {
      ratio = elapsedTime/rampTime;
      Serial.print("Ramp up time: ");
    }else{
      ratio = 1 - (elapsedTime/rampTime);
      Serial.print("Dim Down: ");
    }
    brightness = round(MAXBRIGHTNESS * ratio);
    
    //write to led, may need if statement to switch from DAC to converter pin
    analogWrite(LED, brightness);
    prevTime = currTime;

    
    Serial.print(elapsedTime);
    Serial.print(" Brightness: ");
    Serial.println(brightness);
  }
}
