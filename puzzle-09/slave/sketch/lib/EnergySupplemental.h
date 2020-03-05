/*
  EnergySupplemental.h - Library for controlling energy and syncro reader.
*/
#ifndef EnergySupplemental_h
#define EnergySupplemental_h

#include <Arduino.h>
#include "PowerAdjuster.h"
#include "SyncroReader.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace EnergySupplemental {
  typedef struct {
    PowerAdjuster powerAdjuster;
    SyncroReader syncroReader;
    PowerSwitch powerSwitch;
    Speaker speaker;
    STATE state;
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
  } Components;

  void update(Puzzle & p, Components & c)
  {
    p.registers[REG_SLAVE_ES_MILLIS] = millis();
    p.registers[REG_SLAVE_ES_STATE] = c.state;
    p.registers[REG_SLAVE_ES_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_ES_POWER_ADJUSTER_STATE] = c.powerAdjuster.getState();
    p.registers[REG_SLAVE_ES_SYNCRO_READER_STATE] = c.syncroReader.getState();
    p.registers[REG_SLAVE_ES_SPEAKER_STATE] = c.speaker.getState();

    if (p.registers[REG_MASTER_ES_COMMAND] == CMD_ENABLE &&
        p.registers[REG_SLAVE_ES_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      c.state = ENABLE;
    }

    if (p.registers[REG_MASTER_ES_COMMAND] == CMD_DISABLE &&
        p.registers[REG_SLAVE_ES_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      c.state = DISABLE;
    }

    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.powerSwitch.setState(DISABLE);
      c.powerAdjuster.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      c.state = INITIALIZED;
    }
  }
  void run(Components &c) 
  {
    if (c.state == INITIALIZED) {
      
    }
    if (Serial.available() > 1) {
      keyForPowerAdjuster = Serial.parseInt();
      Serial.println(keyForPowerAdjuster);
    }

    c.powerSwitch.update();
    c.powerAdjuster.update();
    c.syncroReader.update();

    if (c.state == ENABLE) {
      if (c.powerSwitch.getState() == DISABLE) {
        c.powerSwitch.setState(ENABLE);
      }

      if (c.powerSwitch.getState() == OFF) {
        c.powerAdjuster.setState(DISABLE);
        c.syncroReader.setState(DISABLE);
        c.speaker.setState(DISABLE);
      } 

      if (c.powerSwitch.getState() == ON)
      {
        if (c.powerAdjuster.getState() == DISABLE) 
          c.powerAdjuster.setState(ENABLE);
        if (c.speaker.getState() == DISABLE) 
          c.speaker.setState(ENABLE);
      }

      if (c.powerAdjuster.getInputKey() == keyForPowerAdjuster) {
        c.powerAdjuster.setState(SOLVED);
      }
      
      if (c.powerAdjuster.getState() == SOLVED && c.syncroReader.getState() == SOLVED) {
        
      }
    }

    if (c.state == DISABLE) {
      c.powerSwitch.setState(DISABLE);
      c.powerAdjuster.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
    }

    if (c.state == RESET) {
      
    }

    if (c.state == PAUSE) {
      
    }
  }

  void show(Components &c)
  {
    c.showTimer.current = millis();
    if (c.showTimer.current - c.showTimer.showpoint > c.showTimer.interval) {
      c.showTimer.showpoint = millis();
      c.powerSwitch.display();
      c.powerAdjuster.display();
      c.syncroReader.display();
    }
    c.speaker.play();
  }
}

#endif