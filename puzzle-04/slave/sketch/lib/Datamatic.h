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
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
  } Components;

  void update(Puzzle & p, Components & c)
  {
    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_CODE_READER_STATE] = c.codeReader.getState();
    p.registers[REG_SLAVE_INFORMATION_DISPLAY_STATE] = c.informationDisplay.getState();
    p.registers[REG_SLAVE_LIGHT_EFFECT_STATE] = c.lightEffect.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();

    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE && 
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = ENABLE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_DISABLE && 
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = DISABLE;
    }

    if(c.state == SETUP) {
      c.state = INITIALIZING;
      c.powerSwitch.setState(DISABLE);
      c.codeReader.setState(DISABLE);
      c.informationDisplay.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.state = INITIALIZED;
    }
  }

  void run(Components &c)
  {
    if(c.state == INITIALIZED){
      
    }
    
    c.powerSwitch.update();
    c.codeReader.update();
    c.informationDisplay.update();
    c.lightEffect.update();

    if (c.state == ENABLE) {
      Serial.println("ENABLE");
      if (c.powerSwitch.getState() == DISABLE) {
        c.powerSwitch.setState(ENABLE);
      }

      if (c.powerSwitch.getState() == OFF) {
        c.codeReader.setState(DISABLE);
        c.informationDisplay.setState(DISABLE);
        c.lightEffect.setState(DISABLE);
        c.speaker.setState(DISABLE);
      }

      if (c.powerSwitch.getState() == ON) {
        if (c.codeReader.getState() == DISABLE) 
          c.codeReader.setState(ENABLE);
        if (c.informationDisplay.getState() == DISABLE) 
          c.informationDisplay.setState(ENABLE);
        if (c.speaker.getState() == DISABLE) 
          c.speaker.setState(ENABLE);
        if (c.lightEffect.getState() == DISABLE) 
          c.lightEffect.setState(ENABLE);
      }

      if (c.codeReader.getTransmittedKey() == keyForCodeReader1) {
        c.informationDisplay.setCurrentSeries(1);
      } 
      else if (c.codeReader.getTransmittedKey() == keyForCodeReader2) {
        c.informationDisplay.setCurrentSeries(2);
      } 
      else {
        c.informationDisplay.setCurrentSeries(0);
      }
    }

    if (c.state == DISABLE) {
      c.powerSwitch.setState(DISABLE);
      c.codeReader.setState(DISABLE);
      c.informationDisplay.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
      c.speaker.setState(DISABLE);
    }

    if (c.state == PAUSE) {
      
    }

    if (c.state == RESET) {

    }
  }

  void show(Components &c)
  { 
    c.showTimer.current = millis();
    if ((c.showTimer.current - c.showTimer.showpoint) > c.showTimer.interval) {
      c.showTimer.showpoint = millis();
      c.informationDisplay.display();
      c.codeReader.display();
      c.powerSwitch.display();
      c.lightEffect.display();
    }
  }
}

#endif