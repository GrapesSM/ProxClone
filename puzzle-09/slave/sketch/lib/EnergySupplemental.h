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
    unsigned long timer;
    unsigned long lastTime;
  } Components;

  void run(Components c) 
  {
    c.timer = millis();

    if (c.powerSwitch.isSwitchOn()) {
      c.powerSwitch.setLightOn();
    } else {
      c.powerSwitch.setLightOff();
    }

    c.powerAdjuster.update();

    if (c.timer - c.lastTime > 1000) {
      c.lastTime = millis();
      c.powerAdjuster.show();
    }
  }
}

#endif