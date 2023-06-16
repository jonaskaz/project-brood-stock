#include <Arduino.h>

const int led = A2;
const int button = A0;
bool lightState = false;
bool increase = true;
int brightness = 0;

void setup() { 

  pinMode(led, OUTPUT);
  pinMode(button, INPUT);

}

void loop() {

  if(lightState){
    
    analogWrite(led, brightness);
    delay(500);
    if (increase) {
      brightness += 10;
      if(brightness > 255){
        increase = false;
      }
    }else{
      brightness -= 10;
      if(brightness < 0) {
        lightState = false;
      }
    }

  }else if(!lightState){
    if(analogRead(button) == HIGH){
      lightState = true;
      increase = true;
      brightness = 0;
    }
  }
}
