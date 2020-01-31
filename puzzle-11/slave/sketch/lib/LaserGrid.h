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
    if (c.powerSwitch.isSwitchOff()) {
      c.state = OFF;
    }

    if (c.powerSwitch.isSwitchOn()) {
      c.state = ON;
    }

    if (c.state == OFF) {
      c.powerSwitch.setLightOff();
    } 
    
    if (c.state == ON) {
      c.powerSwitch.setLightOn();
    }

    if (c.state == ON) {
      c.keyReader.update();
      if (c.keyReader.isAllInserted()) {
        if (! c.keyReader.isSolved()) {
          c.keyReader.access();
        }
        c.keyReader.setSolved();
      }

    }

    if (c.state == ON) {
      if (c.keyReader.isSolved()) {
        c.waveAdjuster.update();
      }
    }

    if (c.state == ON) {
      if (c.keyReader.isAllInserted() && c.waveAdjuster.isSyncronized()) {
        c.state = SOLVED;
      }
    }
  }

  void show(Components & c)
  {
    c.powerSwitch.display();
    c.waveAdjuster.display();
  }
}

#endif