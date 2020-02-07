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
      if (c.state == ON) c.flag = false;
      c.state = OFF;
      c.powerSwitch.setLightOff();
      c.keyReader.disable();
      if (c.flag == false) {
        c.flag = true;
        // c.speaker.addToPlay(SOUND_POWER_DOWN);
      }
    }

    if (c.powerSwitch.getState() == ON) {
      if (c.state == OFF) c.flag = false;
      c.state = ON;
      c.powerSwitch.setLightOn();
      c.keyReader.enable();
      if (c.flag == false) {
        c.flag = true;
        // c.speaker.addToPlay(SOUND_POWER_UP);
      }
    }

    c.keyReader.update();

    if (c.keyReader.getState() == SOLVED) {
      c.waveAdjuster.enable();
    } else {
      c.waveAdjuster.disable();
    }

    c.waveAdjuster.update();

    if (c.keyReader.getState() == SOLVED) {
      Serial.println(c.waveAdjuster.getInputValue(0));
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