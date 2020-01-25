/*
  Datamatic.h - Library for ____.
*/
#ifndef Datamatic_h
#define Datamatic_h

#include <Arduino.h>
#include "InformationDisplay.h"
#include "CodeReader.h"
#include "PowerSwitch.h"
#include "LightEffect.h"
#include "Speaker.h"

namespace Datamatic {
  typedef struct {
    InformationDisplay informationDisplay;
    CodeReader codeReader;
    PowerSwitch powerSwitch;
    LightEffect lightEffect;
    Speaker speaker;
    STATE state;
  } Components;

  void run(Components &c)
  {
    if (c.powerSwitch.isSwitchOff()) {
      c.state = OFF;
      c.informationDisplay.activate(false);
      c.informationDisplay.disable();
    } else {
      c.state = ON;
      c.informationDisplay.activate();
      c.informationDisplay.enable();
    }

    if (c.state == OFF) {
      c.powerSwitch.setLightOff();
    } 
    
    if (c.state == ON) {
      c.powerSwitch.setLightOn();
    } 

    if (c.state == ON) {
      c.informationDisplay.update();
    } 
    
    if (c.state == ON) {
      if (! c.codeReader.isSolved()) {
        c.codeReader.update();
        if (c.codeReader.getTransmittedKey() == keyForCodeReader) {
          c.codeReader.setSolved();
        }
      } 
    }
    
    c.lightEffect.update();
  }

  void show(Components &c)
  {
    c.informationDisplay.display();
    c.codeReader.display();
    c.powerSwitch.display();
    c.lightEffect.display();
  }
}

#endif