#pragma once
#include "Adafruit_seesaw.h"
#include "Model.h"
#include "View.h"

/*
The controller is responsible for making decisions and intiating actions
Primarily it will deal with inputs from the user

The input will be one rotary encoder that has a push button

Essentially it will take both the model and view and perform actions with them

Main operations:

when rotary encoder is clicked move to the next position

*/

class Controller {
public:
  Adafruit_seesaw ss;
  Controller(int ssAddress, int ssSwitch);
  void run(Model mode, View view);
  int32_t encoderPosition;
  int interruptPin;

private:
  void handleButtonPress(Model model, View view);
  void resetEncoder();
};
