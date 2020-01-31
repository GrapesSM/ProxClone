/*
  LaserGrid.h - Library for _________.
*/
#ifndef LaserGrid_h
#define LaserGrid_h

#include <Arduino.h>
#include "WaveAdjuster.h"
#include "KeyReader.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace LaserGrid {
  typedef struct {
    WaveAdjuster waveAdjuster;
    KeyReader keyReader;
    PowerSwitch powerSwitch;
    Speaker speaker;
    STATE state;
  } Components;

  void run(Components & c) 
  {
    c.powerSwitch.update();
    
    if (c.powerSwitch.getState() == OFF) {
      c.state = OFF;
      c.powerSwitch.setLightOff();
      c.keyReader.disable();
    }

    if (c.powerSwitch.getState() == ON) {
      c.state = ON;
      c.powerSwitch.setLightOn();
      c.keyReader.enable();
    }

    c.keyReader.update();

    if (c.keyReader.getState() == SOLVED) {
      c.waveAdjuster.enable();
    } else {
      c.waveAdjuster.disable();
    }

    c.waveAdjuster.update();

    if (
      c.keyReader.getState() == SOLVED && 
      c.waveAdjuster.getState() == SOLVED
    ) {
      c.state = SOLVED;
    }
  }

  void show(Components & c)
  {
    c.powerSwitch.display();
  }
}

#endif