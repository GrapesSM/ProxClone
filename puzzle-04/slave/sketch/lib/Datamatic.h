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

  void run(Components c)
  {
    if (c.powerSwitch.isSwitchOff()) {
      c.powerSwitch.setLightOff();
      c.informationDisplay.disable();
      c.codeReader.disable();
      c.lightEffect.disable();
      c.speaker.disable();
    } else {
      c.powerSwitch.setLightOff();
      c.informationDisplay.enable();
      c.codeReader.enable();
      c.lightEffect.enable();
      c.speaker.enable();
    }

    if (! c.informationDisplay.isDisabled()) {
      c.informationDisplay.update();
    } 
    
    if (! c.codeReader.isDisabled()) {
      c.codeReader.update();
    }

    if (puzzle.CODE == c.codeReader.getInputValue()) {
      c.lightEffect.setLightOn();
    } else {
      c.lightEffect.setLightOff();
    }
  }

  void show(Components c)
  {
    c.informationDisplay.display();
    c.codeReader.display();
    c.powerSwitch.display();
    c.lightEffect.display();
  }
}

#endif