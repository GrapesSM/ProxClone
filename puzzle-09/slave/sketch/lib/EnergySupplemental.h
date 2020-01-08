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
  } Components;

  void run(Components &c) 
  {
    if (c.powerSwitch.isSwitchOff()) {
      c.state = OFF;
      c.powerSwitch.setLightOff();
      c.powerAdjuster.setDefaultValues();
      c.powerAdjuster.disable();
      c.syncroReader.disable();
      return;
    } else {
      c.state = ON;
      c.powerSwitch.setLightOn();
      c.powerAdjuster.enable();
      c.syncroReader.enable();
    }

    if (! c.powerAdjuster.isDisabled()) {
      if(! c.powerAdjuster.isSolved()){
        c.powerAdjuster.update();
        delay(10);
        c.powerAdjuster.display();
        if(c.powerAdjuster.getInputKey() == keyForPowerAdjuster){
        c.powerAdjuster.setSolved();
        }
      }else{
        c.powerAdjuster.setSolved();
        delay(10);
        c.powerAdjuster.display();
      }  
    }

    if (! c.syncroReader.isDisabled()) {
      if(! c.syncroReader.isSynchronized()){
        c.syncroReader.update();
        if(c.syncroReader.getInputKey() == 1){
          c.syncroReader.setSynchronized();
        }
      }
      else{
        c.syncroReader.setSynchronized();
      }
    }
  }

  void show(Components &c)
  {
    // c.powerAdjuster.display();
    c.powerSwitch.display();
    c.syncroReader.display();
  }
}

#endif