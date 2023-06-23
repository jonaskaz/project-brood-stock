#include <Arduino.h>
#include <Adafruit_BusIO_Register.h>

const int led = 5;
const unsigned long maxMillis = 4294967295;
const int maxBrightness = 255.0;
int brightness;
unsigned long time;

void dimmerControl(double, bool);

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  time = 60000; //1 minute
  brightness = 0;
}

void loop() {
  dimmerControl(time, true);
  dimmerControl(time * 2, false);
}
//ramps up the brightness of the light
//also written to handle millis() overflow, I hope
void dimmerControl(double rampTime, bool increase) {

  unsigned long currTime = millis();
  unsigned long prevTime = currTime;
  double elapsedTime = 0;

  while(elapsedTime < rampTime) {
    currTime = millis();
    //if we have overflowed add from prev time to upper bound,
    //then add from 0 to current time
    if(currTime < prevTime){
      elapsedTime += (maxMillis - prevTime);
      elapsedTime += currTime;
    }else{//add elapsed time 
      elapsedTime += (currTime - prevTime);
    }
    double ratio;
    //dim up or down based on what is needed
    if(increase) {
      ratio = elapsedTime/rampTime;
      Serial.print("Ramp up time: ");
    }else{
      ratio = 1 - (elapsedTime/rampTime);
      Serial.print("Dim Down: ");
    }
    brightness = round(maxBrightness * ratio);
    
    //write to led, may need if statement to switch from DAC to converter pin
    analogWrite(led, brightness);
    prevTime = currTime;

    
    Serial.print(elapsedTime);
    Serial.print(" Brightness: ");
    Serial.println(brightness);
  }
}
