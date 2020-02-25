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
    p.registers[REG_SHIP_ENERGY_SUPPLEMENTAL_POWER_STATE] = c.powerSwitch.isSwitchOn() ? ON : OFF;
    p.registers[REG_SHIP_ENERGY_SUPPLEMENTAL_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_ADJUSTER_STATE] = c.powerAdjuster.getState();
    p.registers[REG_SLAVE_SYNCRO_READER_STATE] = c.syncroReader.getState();
    p.registers[REG_SLAVE_ENERGY_SUPPLEMENTAL_SPEAKER_STATE] = c.speaker.getState();
    p.registers[REG_SLAVE_SYNCRO_READER_INPUT_KEY] = c.syncroReader.getInputKey();

    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE_SYNCRO &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
          c.syncroReader.setState(ENABLE);
          p.registers[REG_MASTER_CONFIRM] = DONE;
      }
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_START_TIMER &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      if (c.syncroReader.getState() == DONE || c.syncroReader.getState() == ENABLE) {
        c.syncroReader.setState(START_TIMER);
        p.registers[REG_MASTER_CONFIRM] = DONE;
      }
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_SYNCRONIZED &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      c.syncroReader.setState(SYNCRONIZED);
      p.registers[REG_MASTER_CONFIRM] = DONE;
    }



    if(c.state == SETUP){
      c.state = INITIALIZING;
      c.powerAdjuster.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.state = INITIALIZED;
    }
  }
  void run(Components &c) 
  {
    if(c.state == INITIALIZED){
      c.state = STANDBY;
      Serial.println("INITIALIZED");
    }
    c.powerSwitch.update();

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

    if(c.state == STANDBY)
    {
      c.powerAdjuster.update();
      c.syncroReader.update();
      c.speaker.update();

      if(c.powerAdjuster.getInputKey() == keyForPowerAdjuster)
      {
        c.powerAdjuster.setState(SOLVED);
      }
      if(c.powerAdjuster.getState() == SOLVED && c.syncroReader.getState() == SOLVED)
      {
        c.setState(SOLVED);
      }
    }

    // if(c.powerSwitch.getState() == ON){
    //   if(c.powerAdjuster.isSolved()){
    //     c.powerAdjuster.setSolved();
    //     delay(10);
    //     c.powerAdjuster.display();
    //   }else{
    //     delay(10);
    //     c.powerAdjuster.display();
    //     if(c.powerAdjuster.getInputKey() == keyForPowerAdjuster){
    //       c.powerAdjuster.setSolved();
    //     }
    //   }
    // }

    // if(c.powerSwitch.getState() == ON){
    //   if(c.syncroReader.isSynchronized()){
    //     c.syncroReader.setSynchronized();
    //   }else{
    //     if(c.syncroReader.getInputKey() == 1){
    //       c.syncroReader.setSynchronized();
    //     }
    //   }
    // }
  }

  void show(Components &c)
  {
    // c.powerAdjuster.display();
    c.powerSwitch.display();
    c.syncroReader.display();
  }
}

#endif