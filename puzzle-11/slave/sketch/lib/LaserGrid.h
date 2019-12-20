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

  void run(Components c) 
  {
    if (c.powerSwitch.isSwitchOff()) {
      c.powerSwitch.setLightOff();
      c.keyReader.disable();
      c.waveAdjuster.disable();
    } else {
      c.powerSwitch.setLightOn();
      c.keyReader.enable();
      c.waveAdjuster.enable();
    }

    if (! c.keyReader.isDisabled()) {
      c.keyReader.update();
    }

    if (! c.waveAdjuster.isDisabled() && c.keyReader.isSolved()) {
      c.waveAdjuster.update();
    }
  }

  void show(Components c)
  {
    c.powerSwitch.display();
    c.waveAdjuster.display();
  }
}

#endif