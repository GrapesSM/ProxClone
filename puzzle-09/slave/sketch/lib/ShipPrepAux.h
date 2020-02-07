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
    c.powerSwitch.update();
    if (c.powerSwitch.getState() == OFF) {
      c.state = OFF;
      c.powerSwitch.setLightOff();
      c.batteryMatrix.disable();
      c.generator.disable();
    } 

    if (c.powerSwitch.getState() == ON)
    {
      c.state = ON;
      c.powerSwitch.setLightOn();
      c.batteryMatrix.enable();
      c.generator.enable();
    }

    c.batteryMatrix.update();
    c.generator.update();

    if(c.state == ON){
      if(c.batteryMatrix.getInputKey() == keyForBatteryMatrix){
        c.batteryMatrix.setSolved();
      }
      if(c.generator.getInputKey() == keyForGenerator){
        c.generator.setSolved();
      }
    }
  }

  void show(Components &c)
  {
    c.powerSwitch.display();
  }
}

#endif