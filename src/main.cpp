#include <Arduino.h>

const int led = 5;
bool increase = true;
int brightness = 0;

void setup() { 

  pinMode(led, OUTPUT);

}

void loop() {

  analogWrite(led, brightness);
  dimUp();
  delay(50);

}

void dimUp(){

  analogWrite(led, brightness);
  if (increase) {
    brightness += 1;
    if(brightness >= 150){
    increase = false;
    }
  }else{
    brightness -= 1;
    if(brightness <= 0) {
      increase = true;
    }
  }

}

void dimDown(){

  analogWrite(led, brightness);
  if (increase) {
    brightness += 1;
    if(brightness >= 150){
    increase = false;
    }
  }else{
    brightness -= 1;
    if(brightness <= 0) {
      increase = true;
    }
  }
  delay(50);

}
