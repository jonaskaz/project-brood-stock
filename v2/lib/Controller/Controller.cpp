#include "Controller.h"
#include "Model.h"
#include "View.h"

Controller::Controller(int ssAddress, int ssSwitch) {
  interruptPin = ssSwitch;
  if (!ss.begin(ssAddress)) {
    Serial.println("Couldn't find rotary encoder");
    while (1)
      delay(10);
  }
  ss.pinMode(interruptPin, INPUT_PULLUP);
  ss.setGPIOInterrupts((uint32_t)1 << interruptPin, 1);
  ss.enableEncoderInterrupt();
  resetEncoder();
}

void Controller::run(Model model, View view) {
  if (!ss.digitalRead(interruptPin)) {
    handleButtonPress(model, view);
    resetEncoder();
  }
  if (view.editMode) {
  }
  int32_t new_position = ss.getEncoderPosition();
  if (encoderPosition != new_position) {
    Serial.println(new_position);
    encoderPosition = new_position;
  }
}

void Controller::resetEncoder() {
  encoderPosition = 0;
  ss.setEncoderPosition(0);
}

void Controller::handleButtonPress(Model model, View view) {
  if (view.currentScreen == home) {
    view.nextScreen();
  } else if (view.editMode) {
    int32_t new_position = ss.getEncoderPosition();
  } else {
    view.editMode = true;
  }
  view.showScreen(model);
}
