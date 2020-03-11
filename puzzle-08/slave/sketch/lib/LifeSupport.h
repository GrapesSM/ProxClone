/*
  LifeSupport.h - Library for _______.
*/
#ifndef LifeSupport_h
#define LifeSupport_h

#include <Arduino.h>
#include "ExternalVent.h"
#include "AirSupplyPump.h"
#include "AirPressureStatus.h"
#include "PowerSwitch.h"
#include "LightEffect.h"
#include "Speaker.h"

namespace LifeSupport {
  typedef struct {
    ExternalVent externalVent;
    AirSupplyPump airSupplyPump;
    AirPressureStatus airPressureStatus;
    PowerSwitch powerSwitch;
    LightEffect lightEffect;
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
    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE &&
        p.registers[REG_SLAVE_CONFIRM] == DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = ENABLE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_DISABLE &&
        p.registers[REG_SLAVE_CONFIRM] == DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = DISABLE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_RESET &&
        p.registers[REG_SLAVE_CONFIRM] == DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = RESET;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_PAUSE &&
        p.registers[REG_SLAVE_CONFIRM] == DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = PAUSE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] == DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = SOLVED;
    }

    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_EXTERNAL_VENT_STATE] = c.externalVent.getState();
    p.registers[REG_SLAVE_AIR_PRESSURE_STATUS_STATE] = c.airPressureStatus.getState();
    p.registers[REG_SLAVE_AIR_SUPPLY_PUMP_STATE] = c.airSupplyPump.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();
    p.registers[REG_SLAVE_LIGHT_EFFECT_STATE] = c.lightEffect.getState();

    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.powerSwitch.setState(DISABLE);
      c.externalVent.setState(DISABLE);
      c.airPressureStatus.setState(DISABLE);
      c.airSupplyPump.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = INITIALIZED;
    }
  }

  void run(Components & c) 
  {
    if (c.state == INITIALIZED) {
      c.state = ENABLE;
    }

    c.powerSwitch.update();
    c.externalVent.update();
    c.airSupplyPump.update();
    c.airPressureStatus.update();
    c.lightEffect.update();
    c.speaker.update();
    
    if (c.state == ENABLE) {
      if (c.powerSwitch.getState() == DISABLE) {
        c.powerSwitch.setState(ENABLE);
      }

      if (c.powerSwitch.getState() == OFF) {
        c.externalVent.setState(DISABLE);
        c.airPressureStatus.setState(DISABLE);
        c.airSupplyPump.setState(DISABLE);
        if (c.speaker.getNumber() != SOUND_POWER_DOWN) {
          c.speaker.addToPlay(SOUND_POWER_DOWN);
        }
        c.lightEffect.setState(DISABLE);
      }
      
      if (c.powerSwitch.getState() == ON) {
        if (c.externalVent.getState() == DISABLE) {
          c.externalVent.setState(ENABLE);
        }
        if (c.airSupplyPump.getState() == DISABLE) {
          c.airSupplyPump.setState(ENABLE);
        }
        if (c.airPressureStatus.getState() == DISABLE) {
          c.airPressureStatus.setState(ENABLE);
        }
        if (c.speaker.getNumber() != SOUND_POWER_UP) {
          c.speaker.addToPlay(SOUND_POWER_UP);
        }
        if (c.lightEffect.getState() == DISABLE) {
          c.lightEffect.setState(ENABLE);
        }
      } 
    
      if (c.externalVent.getState() == OPEN) {
        c.airPressureStatus.decreaseBy(1);
      }
      
      if (c.airSupplyPump.getState() == ON) {
        c.airPressureStatus.increaseBy(1);
      }

      if (
        c.externalVent.getState() == OFF &&
        c.airSupplyPump.getState() == CLOSED &&
        c.airPressureStatus.getState() == BALANCED) {
        c.state = SOLVED;
      }
    }

    if (c.state == SOLVED) {

    }

    if (c.state == DISABLE) {
      c.powerSwitch.setState(DISABLE);
      c.externalVent.setState(DISABLE);
      c.airPressureStatus.setState(DISABLE);
      c.airSupplyPump.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
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
    
      c.lightEffect.display();
      c.powerSwitch.display();
    }
    c.airPressureStatus.display();
    c.speaker.play();
  }
}

#endif