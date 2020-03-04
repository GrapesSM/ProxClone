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
    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_COMBINATION_READER_STATE] = c.combinationReader.getState();
    p.registers[REG_SLAVE_ACCESS_PANEL_STATE] = c.accessPanel.getState();
    p.registers[REG_SLAVE_DOOR_STATE] = c.door.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();

    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = ENABLE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_DISABLE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = DISABLE;
    }

    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.powerSwitch.setState(DISABLE);
      c.combinationReader.setState(DISABLE);
      c.accessPanel.setState(DISABLE);
      c.door.setState(DISABLE);
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
    c.combinationReader.update();
    c.accessPanel.update();
    c.door.update();

    if (c.state == ENABLE) {
      if (c.powerSwitch.getState() == DISABLE) {
        c.powerSwitch.setState(ENABLE);
      }

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

      if (c.accessPanel.getState() == SOLVED) {
        if (c.combinationReader.getState() == DISABLE) {
          c.combinationReader.setState(ENABLE);
        }
      } else {
        c.combinationReader.setState(DISABLE);
      }

      if (c.combinationReader.getState() == SOLVED) {
        if (c.door.getState() == DISABLE) {
          c.door.setState(ENABLE);
        }
      }
    }

    if (c.state == DISABLE) {
      c.powerSwitch.setState(DISABLE);
      c.combinationReader.setState(DISABLE);
      c.accessPanel.setState(DISABLE);
      c.door.setState(DISABLE);
      c.speaker.setState(DISABLE);
    }

    if (c.state == RESET) {

    }

    if (c.state == PAUSE) {

    }

  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if (c.showTimer.current - c.showTimer.showpoint > c.showTimer.interval) {
      c.showTimer.showpoint = millis();
      c.powerSwitch.display();
      c.accessPanel.display();
      c.combinationReader.display();
    }
  }
}

#endif