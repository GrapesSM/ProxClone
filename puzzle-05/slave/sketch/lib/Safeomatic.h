/*
  Safeomatic.h - Library for ____.
*/
#ifndef Safeomatic_h
#define Safeomatic_h

#include <Arduino.h>
#include "CombinationReader.h"
#include "AccessPanel.h"
#include "Door.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace Safeomatic {
  typedef struct {
    PowerSwitch powerSwitch;
    Speaker speaker;
    AccessPanel accessPanel;
    CombinationReader combinationReader;
    Door door;
    STATE state;
    boolean flag;
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
  } Components;

  void update(Puzzle & p, Components & c)
  {
    p.registers[REG_POWER_STATE] = c.powerSwitch.isSwitchOn() ? ON : OFF;
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_COMBINATION_READER_STATE] = c.combinationReader.getState();
    p.registers[REG_SLAVE_ACCESS_PANEL_STATE] = c.accessPanel.getState();
    p.registers[REG_SLAVE_DOOR_STATE] = c.door.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();

    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.combinationReader.setState(DISABLE);
      c.accessPanel.setState(DISABLE);
      c.door.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.state = INITIALIZED;
    }
  }

  void run(Components & c) 
  {
    if (c.state == INITIALIZED) {
      c.state = UNSOLVED;
    }
    c.powerSwitch.update();

    if (c.powerSwitch.getState() == OFF) {
      c.combinationReader.setState(DISABLE);
      c.accessPanel.setState(DISABLE);
      c.door.setState(DISABLE);
      c.speaker.setState(DISABLE);
    } 

    if (c.powerSwitch.getState() == ON) {
      if (c.accessPanel.getState() == DISABLE) 
        c.accessPanel.setState(ENABLE);
      if (c.speaker.getState() == DISABLE) 
        c.speaker.setState(ENABLE);
    }

    if (c.state == UNSOLVED) {
      c.combinationReader.update();
      c.accessPanel.update();
      c.door.update();
      c.speaker.update();

      if (c.accessPanel.getState() == SOLVED) {
        if(c.combinationReader.getState() == DISABLE){
          c.combinationReader.setState(ENABLE);
        }
      }else {
        c.combinationReader.setState(DISABLE);
      }

      if(c.combinationReader.getState() == SOLVED){
        if(c.door.getState() == DISABLE){
          c.door.setState() == ENABLE;
        }
      }
    }
  }

  void show(Components & c)
  {
    c.combinationReader.display();
    c.accessPanel.display();
    c.powerSwitch.display();
  }
}

#endif