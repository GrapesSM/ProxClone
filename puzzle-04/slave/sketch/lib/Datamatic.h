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
    struct Timer {
      unsigned long start = 0;
      unsigned long end = 0;
      unsigned long current = 0;
    } timer;
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
  } Components;

  void update(Puzzle & p, Components & c)
  {

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

    if (p.registers[REG_MASTER_COMMAND] == CMD_RESET &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = RESET;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_LIGHT_EFFECT_PATTERN_NUMBER &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;   
      c.lightEffect.setPatternNumber(p.registers[REG_SLAVE_LIGHT_EFFECT_PATTERN_NUMBER]);
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_DATAMATIC_KEY_1 &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      keyForCodeReader1 = String(p.registers[REG_SLAVE_KEY_1]);
    }    

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_DATAMATIC_KEY_2 &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      keyForCodeReader2 = String(p.registers[REG_SLAVE_KEY_2]);
    }   

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_DATAMATIC_KEY_3 &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      keyForCodeReader3 = String(p.registers[REG_SLAVE_KEY_3]);
    }   

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_DATAMATIC_KEY_4 &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      keyForCodeReader4 = String(p.registers[REG_SLAVE_KEY_4]);
    }  

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_DATAMATIC_KEY_5 &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      keyForCodeReader5 = String(p.registers[REG_SLAVE_KEY_5]);
    }  

    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_CODE_READER_STATE] = c.codeReader.getState();
    p.registers[REG_SLAVE_INFORMATION_DISPLAY_STATE] = c.informationDisplay.getState();
    p.registers[REG_SLAVE_LIGHT_EFFECT_STATE] = c.lightEffect.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();
    p.registers[REG_SLAVE_LIGHT_EFFECT_PATTERN_NUMBER] = c.lightEffect.getPatternNumber();
    //p.registers[REG_SLAVE_KEY_1] = int(keyForCodeReader1);
    //p.registers[REG_SLAVE_KEY_2] = int(keyForCodeReader2);
    //p.registers[REG_SLAVE_KEY_3] = int(keyForCodeReader3);
    //p.registers[REG_SLAVE_KEY_4] = int(keyForCodeReader4);
    //p.registers[REG_SLAVE_KEY_5] = int(keyForCodeReader5);


    if(c.state == SETUP) {
      c.state = INITIALIZING;
      p.registers[REG_SLAVE_CONFIRM] = DONE;

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
    if(c.state == INITIALIZED) {
      c.state = ENABLE;
    }
    
    c.powerSwitch.update();
    c.codeReader.update();
    c.informationDisplay.update();
    c.lightEffect.update();

    if (c.state == ENABLE) {
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
      else if (c.codeReader.getTransmittedKey() == keyForCodeReader3) {
        c.informationDisplay.setCurrentSeries(3);
      }
      else if (c.codeReader.getTransmittedKey() == keyForCodeReader4) {
        c.informationDisplay.setCurrentSeries(4);
      }
      else if (c.codeReader.getTransmittedKey() == keyForCodeReader5) {
        c.informationDisplay.setCurrentSeries(5);
      }else{
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
      c.state = SETUP;
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