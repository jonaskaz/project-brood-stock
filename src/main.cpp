#include <Arduino.h>

#include <ezButton.h>

#define CLOCKPIN 7

ezButton clock(CLOCKPIN);

bool isClockOn();

void setup() {
  Serial.begin(9600);
  clock.setDebounceTime(200);
}

void loop() {
  clock.loop();

  if (isClockOn()) {
    Serial.println("Lights should be on");
  } else {
    Serial.println("Off");
  }
  delay(50);
}

bool isClockOn() {
  int state = clock.getState();
  return (state == LOW);
}
