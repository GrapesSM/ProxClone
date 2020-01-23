/*
  Safeomatic.h - Library for ____.
*/
#ifndef Safeomatic_h
#define Safeomatic_h

#include <Arduino.h>
#include "CombinationReader.h"
#include "Door.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace Safeomatic {
  typedef struct {
    CombinationReader combinationReader;
    Door door;
    PowerSwitch powerSwitch;
    Speaker speaker;
    STATE state;
  } Components;

  void run(Components c) 
  {
    if (c.powerSwitch.isSwitchOff()) {
      c.powerSwitch.setLightOff();
      c.combinationReader.disable();
    } else {
      c.powerSwitch.setLightOn();
      c.combinationReader.enable();
    }

    if (! c.combinationReader.isDisabled()) {
      c.combinationReader.update();
    } 

    if (! c.combinationReader.isCorrect() && c.door.isClosed()) {
      c.door.open();
    } else {
      c.door.close();
    }
  }

  void show(Components c)
  {
    c.combinationReader.display();
    c.powerSwitch.display();
  }
}

#endif