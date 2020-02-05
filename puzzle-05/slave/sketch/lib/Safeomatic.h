/*
  Safeomatic.h - Library for ____.
*/
#ifndef Safeomatic_h
#define Safeomatic_h

#include <Arduino.h>
#include "CombinationReader.h"
#include "AccessPanel.h"
#include "Door.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace Safeomatic {
  typedef struct {
    PowerSwitch powerSwitch;
    Speaker speaker;
    AccessPanel accessPanel;
    CombinationReader combinationReader;
    Door door;
    STATE state;
  } Components;

  void run(Components & c) 
  {
    if (c.powerSwitch.isSwitchOff()) {
      c.state = OFF;
    } else {
      c.state = ON;
    }

    if (c.state == OFF) {
      c.powerSwitch.setLightOff();
      //c.door.close();
      //c.combinationReader.disable();
    }

    if (c.state == ON) {
      c.powerSwitch.setLightOn();
      if (c.accessPanel.keyInserted()) {
        //Serial.println("key inserted");
        if(c.accessPanel.isClosed()) {
          Serial.println("opening access panel");
          c.accessPanel.open();
          // if (c.combinationReader.isDisabled) {
          c.combinationReader.enable();
          c.combinationReader.setLightsRed();
          // }
        
        } 
      } else {
          c.accessPanel.close();
          c.combinationReader.disable();
      }

      if (!c.combinationReader.isDisabled()&&!c.combinationReader.isSolved()) {
        c.combinationReader.update();
        //c.combinationReader.checkDirection();
        c.combinationReader.checkNumber();
      }

      if (c.combinationReader.isSolved()) {
        if(c.door.isClosed()) {
          c.door.open();
        }
      } else {
        c.door.close();
      }
    }


  }

  void show(Components & c)
  {
    c.combinationReader.display();
    c.accessPanel.display();
    c.powerSwitch.display();
  }
}

#endif