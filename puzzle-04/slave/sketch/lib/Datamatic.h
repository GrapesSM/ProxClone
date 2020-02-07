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
    c.powerSwitch.update();
    if (c.powerSwitch.getState() == OFF) {
      c.state = OFF;
      c.powerSwitch.setLightOff();
      c.codeReader.disable();
      c.informationDisplay.disable();
    } 
    if (c.powerSwitch.getState() == ON)
    {
      c.state = ON;
      c.powerSwitch.setLightOn();
      c.codeReader.enable();
      c.informationDisplay.enable();
    }

    c.codeReader.update();
    c.informationDisplay.update();
    c.lightEffect.update();

    if (c.state == ON) {
      if (c.codeReader.getTransmittedKey() == keyForCodeReader1){
        c.informationDisplay.setCurrentSeries(1);
      }
      if (c.codeReader.getTransmittedKey() == keyForCodeReader2) {
        c.informationDisplay.setCurrentSeries(2);
      } 
    }
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