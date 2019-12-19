/*
  PowerControl.h - Library for controlling energy and syncro reader.
*/
#ifndef PowerControl_h
#define PowerControl_h

#include <Arduino.h>
#include "PowerAdjuster.h"
#include "SyncroReader.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace PowerControl {
  typedef struct {
    PowerAdjuster powerAdjuster;
    PowerLightIndicator powerLightIndicator;
    PowerPinIndicator powerPinIndicator;
    PowerBarIndicator powerBarIndicator;
    LightEffect lightEffect;
    Speaker speaker;
    STATE state;
    float currentDemand = 0.0;
    float currentSupply = 0.0;
    float maxDemand = 13.0;
    unsigned long timer;
  } Components;

  void run(Components c) 
  {
    c.timer = millis();

    if (powerAdjuster.isBalanced()) {
      powerLightIndicator.turnToWhite();
    } else {
      powerLightIndicator.turnToRed();
    }

    powerAdjuster.setOutputValue(c.currentSupply);
    // read register and add to current total value
    powerAdjuster.update();
    
    powerBarIndicator.setValue(powerControl.getInputValue());
    powerPinIndicator.setValue(powerControl.getInputValue());

    if (puzzle.MAX_POWER >= powerControl.getInputValue()) {
      // shut off all puzzles by setting register to SHUT_OFF_ALL_PUZZLES state
    }
  }

  void show(Components c)
  {
    powerAdjuster.display();
    powerLightIndicator.display();
    powerBarIndicator.display();
    powerPinIndicator.display();
    lightEffect.display();
  }
}

#endif