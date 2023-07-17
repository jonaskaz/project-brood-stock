#include "Controller.h"
#include "Model.h"
#include "View.h"

Controller::Controller() {}

void Controller::init(int ssAddress, int ssSwitch) {
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

void Controller::run(Model &model, View &view) {
  if (!ss.digitalRead(interruptPin)) {
    handleButtonPress(model, view);
    resetEncoder();
    model.saveModel();
  }
  if (view.editMode) {
    updateModelFromEncoder(model, view);
  }
  view.showScreen(model);
}

void Controller::handleButtonPress(Model &model, View &view) {
  if (view.currentScreen == home) {
    view.nextScreen();
  } else if (!view.editMode) {
    view.editMode = true;
  } else {
    view.nextScreen();
    view.editMode = false;
  }
  view.lcd.clear();
  view.showScreen(model);
  delay(300);
}

void Controller::updateModelFromEncoder(Model &model, View &view) {
  if (getEncoderDiff() == 0) {
    return;
  }
  switch (view.currentScreen) {
  case home:
    break;
  case currentTime:
    break;
  case currentBrightness:
    break;
  case manualSunrise:
    model.manualSunriseTime += (getEncoderDiff() * 60);
    break;
  case manualSunset:
    model.manualSunsetTime += (getEncoderDiff() * 60);
    break;
  case manualMode:
    model.manualTiming = !model.manualTiming;
    break;
  case sunriseLength:
    model.sunriseLength += getEncoderDiff();
    model.sunriseLength = constrain(model.sunriseLength, 0, 3600);
    break;
  case sunsetLength:
    model.sunsetLength += getEncoderDiff();
    model.sunsetLength = constrain(model.sunsetLength, 0, 3600);
    break;
  case maxBrightnessPercent:
    model.maxBrightnessPercent += getEncoderDiff();
    model.maxBrightnessPercent = constrain(model.maxBrightnessPercent, 0, 100);
    break;
  default:
    break;
  }
  resetEncoder();
  view.lcd.print("                   ");
}

void Controller::resetEncoder() {
  encoderStartPos = 0;
  ss.setEncoderPosition(0);
}

int32_t Controller::getEncoderDiff() {
  int32_t new_position = ss.getEncoderPosition();
  return new_position;
}
