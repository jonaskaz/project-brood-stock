#include <Arduino.h>
#include <Wire.h>
#include <ezButton.h>

#define CLOCKPIN 7
#define PWMPIN 3

ezButton clock(CLOCKPIN);

const int MAXBRIGHTNESS = 255;
const int MINBRIGHTNESS = 0;
const int INCREMENT = 10;
const long RAMPTIME = 10000;

int brightness = MINBRIGHTNESS;
int startBrightness = brightness;
unsigned long elapsedTime = 0;
unsigned long startTime;

bool isClockOn();
void setPWMValue(int value);
void updateSunriseBrightness();
void updateSunsetBrightness();
void updateElapsedTime();

void setup() {
  Serial.begin(115200);
  clock.setDebounceTime(200);
  startTime = millis();
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
    setPWMValue(brightness);
  }
  Serial.print("Brightness: ");
  Serial.println(brightness);
}

bool isClockOn() {
  int state = clock.getState();
  return (state == LOW);
}

void setPWMValue(int value) {
  value = constrain(value, MINBRIGHTNESS, MAXBRIGHTNESS);
  analogWrite(PWMPIN, value);
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
