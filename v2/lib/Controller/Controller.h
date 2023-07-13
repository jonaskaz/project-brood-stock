#pragma once
#include "Model.h"
#include "View.h"
#include <Adafruit_seesaw.h>

class Controller {
public:
  Adafruit_seesaw ss;
  Controller();
  /**
   * Begins encoder and enables interrupt pin. Sets initial encoder value.
   *
   * @param ssAddress the address for the rotary encoder
   * @param ssSwtich the pin for the encoder interrupt switch
   */
  void init(int ssAddress, int ssSwitch);
  /**
   * Continuously show the currentScreen. If editing is enabled then
   * continuously update the model based on encoder data. Change screens, move
   * cursor, and reset encoder values on when button is pressed.
   *
   * @param model the data model
   * @param view the lcd screen view
   */
  void run(Model &model, View &view);
  int32_t encoderStartPos;
  int interruptPin;

private:
  /**
   * Depending on current state, either update the view to the next screen, or
   * move into edit mode. Shows the currentScreen using view and delays to
   * prevent button bouncing.
   *
   * @param model the data model
   * @param view the lcd screen view
   */
  void handleButtonPress(Model &model, View &view);
  /**
   * Based on the currentScreen, updates the model with encoder values. Resets
   * the encoder and clears the current line on the lcd to prepare for the new
   * data.
   *
   * @param model the data model
   * @param view the lcd screen view
   */
  void updateModelFromEncoder(Model &model, View &view);
  /**
   * Sets the encoderStartPos member to 0 and sets encoder value to 0
   */
  void resetEncoder();
  /**
   * Returns the current encoder position. Assumes that the start position of
   * the encoder is 0.
   *
   * @return newPosition the current encoder position
   */
  int32_t getEncoderDiff();
};
