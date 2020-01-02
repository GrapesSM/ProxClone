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
  } Components;

  void run(Components &c) 
  {
    if (c.powerSwitch.isSwitchOff()) {
      c.state = OFF;
      c.powerSwitch.setLightOff();
      c.batteryMatrix.disable();
      c.generator.disable();
    } else {
      c.state = ON;
      c.powerSwitch.setLightOn();
      c.batteryMatrix.enable();
      c.generator.enable();
    }

    if (! c.batteryMatrix.isDisabled()) {
      c.batteryMatrix.update();
    }

    if (! c.generator.isDisabled()) {    
      c.generator.update();
    }

  }

  void show(Components &c)
  {
    c.powerSwitch.display();
  }
}

#endif