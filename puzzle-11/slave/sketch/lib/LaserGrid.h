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
    boolean in;
    boolean flag;
  } Components;

  void run(Components & c) 
  {
    c.powerSwitch.update();
    
    if (c.powerSwitch.getState() == OFF) {
      if (c.state == ON) {
        c.speaker.addToPlay(SOUND_POWER_UP);
      }
      c.state = OFF;
      c.powerSwitch.setLightOff();
      c.keyReader.disable();
    }

    if (c.powerSwitch.getState() == ON) {
      if (c.state == OFF) {
        c.speaker.addToPlay(SOUND_POWER_UP);
      }
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

    if (c.keyReader.getState() == SOLVED) {
      c.speaker.speak(c.waveAdjuster.getInputValue(0));
    }

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
    c.speaker.play();
  }
}

#endif