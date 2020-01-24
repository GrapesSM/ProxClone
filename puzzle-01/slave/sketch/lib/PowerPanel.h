/*
  PowerControl.h - Library for controlling energy and syncro reader.
*/
#ifndef PowerControl_h
#define PowerControl_h

#include <Arduino.h>
#include "PowerAdjuster.h"
#include "PowerLightIndicator.h"
#include "PowerBarIndicator.h"
#include "LightEffect.h"
#include "Speaker.h"

namespace PowerPanel {
  typedef struct {
    PowerAdjuster powerAdjuster;
    PowerLightIndicator powerLightIndicator;
    PowerBarIndicator powerBarIndicator;
    LightEffect lightEffect;
    Speaker speaker;
    STATE state;
    float currentDemand = 0.0;
    float currentSupply = 0.0;
    float maxDemand = 7.0;
    unsigned long timer;
  } Components;

  void run(Components & c) 
  {
    c.timer = millis();

    if (! c.state) {
      c.state = OFF;
    }

    if (c.state == OFF) {
      c.powerLightIndicator.setOn();
      c.powerBarIndicator.setMaxValue(c.maxDemand);
      c.state = ON;
    }

    if (c.powerAdjuster.isBalanced()) {
      
    } else {
      
    }

    // read register and add to current total value
    c.powerAdjuster.setOutputValue(7);
    c.powerAdjuster.update();
    c.powerBarIndicator.setValue(c.powerAdjuster.getInputValue()); // 
    c.powerBarIndicator.update();

    c.lightEffect.update();

    // if (puzzle.MAX_POWER >= powerControl.getInputValue()) {
    //   // shut off all puzzles by setting register to SHUT_OFF_ALL_PUZZLES state
    // }
  }

  void show(Components & c)
  {
    c.powerAdjuster.display();
    c.powerLightIndicator.display();
    // c.powerBarIndicator.display();
    c.lightEffect.display();
  }
}

#endif