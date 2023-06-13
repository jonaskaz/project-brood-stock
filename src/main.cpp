#include <Arduino.h>

const int pwm = A2;
int brightness = 0;

void setup() { pinMode(pwm, OUTPUT); }

void loop() {

  analogWrite(pwm, brightness);
  delay(500);
  brightness += 10;
  if (brightness > 255) {
    brightness = 0;
  }
}
