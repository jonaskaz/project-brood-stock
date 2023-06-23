#include <Arduino.h>
#include <Adafruit_BusIO_Register.h>

const int led = 5;
const unsigned long maxMillis = 4294967295;
const int maxBrightness = 255.0;
int brightness;
unsigned long time;

void dimUp(double);
void dimDown(unsigned long);

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  time = 60000; //10 minutes
  brightness = 0;
}

void loop() {
  dimUp(time);
  dimDown(time*2);
}
//ramps up the brightness of the light
//also written to handle millis() overflow, I hope
void dimUp(double rampTime) {

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
    double ratio = elapsedTime/rampTime;
    brightness = round(maxBrightness * ratio);
    
    //write to led, may need if statement to switch from DAC to converter pin
    analogWrite(led, brightness);
    prevTime = currTime;

    Serial.print("Ramp up time: ");
    Serial.print(elapsedTime);
    Serial.print(" Brightness: ");
    Serial.println(brightness);
  }
}

void dimDown(unsigned long dimTime){

  unsigned long currTime = millis();
  unsigned long prevTime = currTime;
  double elapsedTime = 0;

  while(elapsedTime < dimTime) {
    currTime = millis();
    //if we have overflowed add from prev time to upper bound,
    //then add from 0 to current time
    if(currTime < prevTime){
      elapsedTime += (maxMillis - prevTime);
      elapsedTime += currTime;
    }else{//add elapsed time 
      elapsedTime += (currTime - prevTime);
    }
    double ratio = 1 - (elapsedTime/dimTime);
    brightness = round(maxBrightness * ratio);
    
    //write to led, may need if statement to switch from DAC to converter pin
    analogWrite(led, brightness);
    prevTime = currTime;

    Serial.print("Dim down time: ");
    Serial.print(elapsedTime);
    Serial.print(" Brightness: ");
    Serial.println(brightness);
  }

}
