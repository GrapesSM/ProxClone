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
  } Components;

  void run(Components components) 
  {
    if (components.powerSwitch.isSwitchOn()) {
      components.powerSwitch.setLightOn();
    } else {
      components.powerSwitch.setLightOff();
    }
  }
}

#endif