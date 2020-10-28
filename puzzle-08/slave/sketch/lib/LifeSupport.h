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
    int powerSwitchStateChange[2] = {0, 0}; // old, new
    int externalVentStateChange[2] = {0, 0}; // old, new
    int airSupplyPumpStateChange[2] = {0, 0}; // old, new
  } Components;

  void update(Puzzle & p, Components & c) 
  {
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

    if (p.registers[REG_MASTER_COMMAND] == CMD_RESET &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = RESET;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
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
    p.registers[REG_SLAVE_AIR_PRESSURE_STATUS_VALUE] = c.airPressureStatus.getValue();

    if (c.state == SETUP) {
      c.state = INITIALIZING;
      
      c.airPressureStatus.init();
      c.externalVent.init();
      c.lightEffect.init();
      c.airSupplyPump.init();
      c.powerSwitch.init();
      
      c.powerSwitch.setState(DISABLE);
      c.externalVent.setState(DISABLE);
      c.airPressureStatus.setState(DISABLE);
      c.airSupplyPump.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = ENABLE;
    }
  }

  void run(Components & c) 
  {
    c.powerSwitch.update();
    c.externalVent.update();
    c.airSupplyPump.update();
    c.airPressureStatus.update();
    c.lightEffect.update();
    c.speaker.update();

    if (c.powerSwitch.getState() == DISABLE) {
      c.powerSwitch.setState(ENABLE);
    }

    if (c.state == DISABLE) {
      c.powerSwitch.setLightOff();
      c.externalVent.setState(DISABLE);
      c.airPressureStatus.setState(DISABLE);
      c.airSupplyPump.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
    }

    if (c.state == RESET) {
      c.state = SETUP;
    }    
    
    if (c.state == ENABLE) {
      if (c.powerSwitch.getState() == OFF) {
        c.externalVent.setState(DISABLE);
        c.airPressureStatus.setState(DISABLE);
        c.airSupplyPump.setState(DISABLE);
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
        if (c.lightEffect.getState() == DISABLE) {
          c.lightEffect.setState(ENABLE);
        }
        if (c.speaker.getState() == DISABLE) {
          c.speaker.setState(ENABLE);
        }
      } 
    
      if (c.externalVent.getState() == OPEN) {
        c.airPressureStatus.decreaseBy(1);
      }
      
      if (c.airSupplyPump.getState() == ON) {
        c.airPressureStatus.increaseBy(1);
      }

      if (
        c.externalVent.getState() == CLOSED &&
        c.airSupplyPump.getState() == OFF &&
        c.airPressureStatus.getState() == BALANCED) {
        c.state = SOLVED;
      }
    }

    if (c.state == SOLVED) {
      if (c.powerSwitch.getState() == OFF) {
        c.externalVent.setState(DISABLE);
        c.airPressureStatus.setState(DISABLE);
        c.airSupplyPump.setState(DISABLE);
        c.speaker.setState(DISABLE);
        c.lightEffect.setState(DISABLE);
      } 

      if (c.powerSwitch.getState() == ON) {
        c.externalVent.setState(ENABLE);
        c.airPressureStatus.setState(ENABLE);
        c.airSupplyPump.setState(ENABLE);
        c.speaker.setState(ENABLE);
        c.lightEffect.setState(SOLVED);
      }
    }
  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if ((c.showTimer.current - c.showTimer.showpoint) > c.showTimer.interval) {
      c.showTimer.showpoint = millis();

      // Code here runs every interval (i.e. 200ms)
      c.airPressureStatus.display();
    }

    c.lightEffect.display();
    c.powerSwitch.display();
  }

  void sound(Components & c)
  {
    c.powerSwitchStateChange[1] = c.powerSwitch.getState();
    if (c.powerSwitchStateChange[0] != c.powerSwitchStateChange[1]) {      
      c.powerSwitchStateChange[0] = c.powerSwitchStateChange[1];
      if (c.powerSwitch.getState() == ON) {
        c.speaker.setCurrent(SOUND_STATION_UP);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }

      if (c.powerSwitch.getState() == OFF) {
        c.speaker.setCurrent(SOUND_STATION_DOWN);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }
    } else {
      c.externalVentStateChange[1] = c.externalVent.getState();
      if (c.externalVentStateChange[0] != c.externalVentStateChange[1]) {      
        c.externalVentStateChange[0] = c.externalVentStateChange[1];
        if (c.externalVent.getState() == OPEN) {
          c.speaker.setCurrent(SOUND_EXTERNAL_VENT);
          c.speaker.setRepeat(true);
          c.speaker.setPlayPartly(true);
        }

        if (c.externalVent.getState() == CLOSED) {
          c.speaker.setRepeat(false);
          c.speaker.setPlayPartly(false);
          c.speaker.soundOff();
        }
      }

      if (c.externalVent.getState() == CLOSED) {
        c.airSupplyPumpStateChange[1] = c.airSupplyPump.getState();
        if (c.airSupplyPumpStateChange[0] != c.airSupplyPumpStateChange[1]) {      
          c.airSupplyPumpStateChange[0] = c.airSupplyPumpStateChange[1];
          if (c.airSupplyPump.getState() == ON) {
            c.speaker.setCurrent(SOUND_AIR_SUPPLY_PUMP);
            c.speaker.setRepeat(true);
            c.speaker.setPlayPartly(true);
          }

          if (c.airSupplyPump.getState() == OFF) {
            c.speaker.setRepeat(false);
            c.speaker.setPlayPartly(false);
            c.speaker.soundOff();
          }
        }
      }
    }


    if (c.speaker.getPlayPartly()) {
      c.speaker.playBytes(1024);
    } else {
      c.speaker.play();
    }
  }
}

#endif