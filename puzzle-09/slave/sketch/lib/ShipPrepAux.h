/*
  ShipPrepAux.h - Library for controlling energy and syncro reader.
*/
#ifndef ShipPrepAux_h
#define ShipPrepAux_h

#include <Arduino.h>
#include "BatteryMatrix.h"
#include "Generator.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace ShipPrepAux {
  typedef struct {
    BatteryMatrix batteryMatrix;
    Generator generator;
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
    p.registers[REG_SHIP_PREP_AUX_POWER_STATE] = c.powerSwitch.isSwitchOn() ? ON : OFF;
    p.registers[REG_SHIP_PREP_AUX_STATE] = c.state;
    p.registers[REG_SLAVE_BATTERY_MATRIX_STATE] = c.batteryMatrix.getState();
    p.registers[REG_SLAVE_GENERATOR_STATE] = c.generator.getState();
    p.registers[REG_SLAVE_SHIP_PREP_AUX_SPEAKER_STATE] = c.speaker.getState();

    if(c.state == SETUP){
      c.state = INITIALIZING;
      c.batteryMatrix.setState(DISABLE);
      c.generator.setState(DISABLE);
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
      c.batteryMatrix.setState(DISABLE);
      c.generator.setState(DISABLE);
      c.speaker.setState(DISABLE);
    } 

    if (c.powerSwitch.getState() == ON)
    {
      if (c.batteryMatrix.getState() == DISABLE) 
        c.batteryMatrix.setState(ENABLE);
      if (c.generator.getState() == DISABLE) 
        c.generator.setState(ENABLE);
      if (c.speaker.getState() == DISABLE) 
        c.speaker.setState(ENABLE);
    }

    if(c.state == STANDBY)
    {
      c.batteryMatrix.update();
      c.generator.update();
      c.speaker.update();

      if(c.batteryMatrix.getInputKey() == keyForBatteryMatrix)
      {
        c.batteryMatrix.setState(SOLVED);
      }
      if(c.generator.getInputKey() == keyForGenerator)
      {
        c.generator.setState(SOLVED);
      }
    }
  }

  void show(Components &c)
  {
    c.powerSwitch.display();
  }
}

#endif