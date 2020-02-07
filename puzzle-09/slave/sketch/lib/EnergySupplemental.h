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
    c.powerSwitch.update();
    if (c.powerSwitch.getState() == OFF) {
      c.state = OFF;
      c.powerSwitch.setLightOff();
      c.powerAdjuster.setDefaultValues();
      c.powerAdjuster.disable();
      c.syncroReader.disable();
      return;
    } 

    if(c.powerSwitch.getState() == ON){
      c.state = ON;
      c.powerSwitch.setLightOn();
      c.powerAdjuster.enable();
      c.syncroReader.enable();
    }  

    c.powerAdjuster.update();
    c.syncroReader.update();

    if(c.powerSwitch.getState() == ON){
      if(c.powerAdjuster.isSolved()){
        c.powerAdjuster.setSolved();
        delay(10);
        c.powerAdjuster.display();
      }else{
        delay(10);
        c.powerAdjuster.display();
        if(c.powerAdjuster.getInputKey() == keyForPowerAdjuster){
          c.powerAdjuster.setSolved();
        }
      }
    }

    if(c.powerSwitch.getState() == ON){
      if(c.syncroReader.isSynchronized()){
        c.syncroReader.setSynchronized();
      }else{
        if(c.syncroReader.getInputKey() == 1){
          c.syncroReader.setSynchronized();
        }
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