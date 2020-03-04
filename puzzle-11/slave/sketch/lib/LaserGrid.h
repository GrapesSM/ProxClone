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
    struct Timer {
      unsigned long start = 0;
      unsigned long end = 0;
      unsigned long current = 0;
    } timer;
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
  } Components;

  void update(Puzzle & p, Components & c) 
  {
    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_KEY_READER_STATE] = c.keyReader.getState();
    p.registers[REG_SLAVE_WAVE_ADJUSTER_STATE] = c.waveAdjuster.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();
    
    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.powerSwitch.setState(DISABLE);
      c.keyReader.setState(DISABLE);
      c.waveAdjuster.setState(DISABLE);
      c.speaker.setState(DISABLE);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = INITIALIZED;
    }
  }

  void run(Components & c) 
  {
    if (c.state == INITIALIZED) {
        
    }

    c.powerSwitch.update();
    c.keyReader.update();
    c.waveAdjuster.update();

    if (c.state == ENABLE) {
      if (c.powerSwitch.getState() == DISABLE) {
        c.powerSwitch.setState(ENABLE);
      }

      if (c.powerSwitch.getState() == OFF) {
        if (c.state == ON) {
          // c.speaker.addToPlay(SOUND_POWER_UP);
        }

        c.keyReader.setState(DISABLE);
        c.waveAdjuster.setState(DISABLE);
        c.speaker.setState(DISABLE);
      }

      if (c.powerSwitch.getState() == ON) {
        if (c.state == OFF) {
          // c.speaker.addToPlay(SOUND_POWER_UP);
        }

        if (c.keyReader.getState() == DISABLE)
          c.keyReader.setState(ENABLE);
        if (c.speaker.getState() == DISABLE)
          c.speaker.setState(ENABLE);
      }

      if (c.keyReader.getState() == SOLVED) {
        c.waveAdjuster.setState(ENABLE);
      }
 
      if (
        c.keyReader.getState() == SOLVED && 
        c.waveAdjuster.getState() == SOLVED
      ) {
        c.state = SOLVED;
      }
    }
    

    if (c.state == SOLVED) {
      Serial.println("SOLVED");
    }

    if (c.state == RESET) {

    }

    if (c.state == PAUSE) {

    }
  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if ((c.showTimer.current - c.showTimer.showpoint) > c.showTimer.interval) {
      c.showTimer.showpoint = millis();

      c.powerSwitch.display();
      c.speaker.play();
    }
    
    c.waveAdjuster.display();
  }
}

#endif