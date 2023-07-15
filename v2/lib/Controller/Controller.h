#pragma once
#include "Model.h"
#include "View.h"
#include <Adafruit_seesaw.h>

class Controller {
public:
  Adafruit_seesaw ss;
  Controller();
  /**
   * Begin encoder and enable interrupt pin. Set initial encoder value.
   *
   * @param ssAddress the address for the rotary encoder
   * @param ssSwtich the pin for the encoder interrupt switch
   */
  void init(int ssAddress, int ssSwitch);
  /**
   * Continuously show the currentScreen. If editing is enabled then
   * continuously update the model based on encoder data. When button is
   * pressed: change screens, move cursor, and reset encoder value.
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
   * move into edit mode. Show the currentScreen using view and delay to
   * prevent button bouncing.
   *
   * @param model the data model
   * @param view the lcd screen view
   */
  void handleButtonPress(Model &model, View &view);
  /**
   * Based on the currentScreen, update the model with encoder values. Reset
   * the encoder and clear the current line on the lcd to prepare for new
   * data.
   *
   * @param model the data model
   * @param view the lcd screen view
   */
  void updateModelFromEncoder(Model &model, View &view);
  /**
   * Set the encoderStartPos member to 0 and set encoder value to 0
   */
  void resetEncoder();
  /**
   * Return the current encoder position. Assume that the start position of
   * the encoder is 0.
   *
   * @return newPosition the current encoder position
   */
  int32_t getEncoderDiff();
};
