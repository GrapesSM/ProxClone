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
    p.registers[REG_POWER_STATE] = c.powerSwitch.isSwitchOn() ? ON : OFF;
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_EXTERNAL_VENT_STATE] = c.externalVent.getState();
    p.registers[REG_SLAVE_AIR_PRESSURE_STATUS_STATE] = c.airPressureStatus.getState();
    p.registers[REG_SLAVE_AIR_SUPPLY_PUMP_STATE] = c.airSupplyPump.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();
    p.registers[REG_SLAVE_LIGHT_EFFECT_STATE] = c.lightEffect.getState();
    
    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.externalVent.setState(DISABLE);
      c.airPressureStatus.setState(DISABLE);
      c.airSupplyPump.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
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
      c.lightEffect.setState(ENABLE);
    } 
   
    if (c.state == UNSOLVED) {
      c.externalVent.update();
      c.airSupplyPump.update();
      c.airPressureStatus.update();

      if (c.externalVent.getState() == ON) {
        c.airPressureStatus.decreaseBy(1);
      }
      
      if (c.airSupplyPump.getState() == OPEN) {
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
      Serial.println("SOLVED");
    }
  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if ((c.showTimer.current - c.showTimer.showpoint) > c.showTimer.interval) {
      c.showTimer.showpoint = millis();
    
      c.powerSwitch.display();
      c.lightEffect.display();
    }
    c.airPressureStatus.display();
  }
}

#endif