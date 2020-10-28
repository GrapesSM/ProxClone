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
    int powerSwitchStateChange[2] = {0, 0}; // old, new
    int codeReaderModeChange[2] = {0, 0}; // old, new
    String codeReaderTransmitChange[2] = {String(""), String("")}; // old, new
    char codeReaderInputButtonsChange[2] = {'n', 'n'};
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

    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_CODE_READER_STATE] = c.codeReader.getState();
    p.registers[REG_SLAVE_INFORMATION_DISPLAY_STATE] = c.informationDisplay.getState();
    p.registers[REG_SLAVE_LIGHT_EFFECT_STATE] = c.lightEffect.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();

    if(c.state == SETUP) {
      c.state = INITIALIZING;
      p.registers[REG_SLAVE_CONFIRM] = DONE;

      c.powerSwitch.init();
      c.codeReader.init();
      c.informationDisplay.init();
      c.lightEffect.init();
      c.speaker.init();

      c.powerSwitch.setState(DISABLE);
      c.codeReader.setState(DISABLE);
      c.informationDisplay.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.state = ENABLE;
    }
  }

  void run(Components &c)
  {
    c.powerSwitch.update();
    c.codeReader.update();
    c.informationDisplay.update();
    c.lightEffect.update();

    if (c.powerSwitch.getState() == DISABLE) {
      c.powerSwitch.setState(ENABLE);
    }

    if (c.state == DISABLE) {
      c.powerSwitch.setLightOff();
      c.codeReader.setState(DISABLE);
      c.informationDisplay.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
      c.speaker.setState(DISABLE);
    }

    if (c.state == RESET) {
      c.state = SETUP;
    }

    if (c.state == ENABLE) {

      if (c.powerSwitch.getState() == OFF) {
        c.codeReader.setState(DISABLE);
        c.informationDisplay.setState(DISABLE);
        c.lightEffect.setState(DISABLE);
      }

      if (c.powerSwitch.getState() == ON) {
        if (c.codeReader.getState() == DISABLE) {
          c.codeReader.setState(ENABLE);
        }
        if (c.informationDisplay.getState() == DISABLE) {
          c.informationDisplay.setState(ENABLE);
        } 
        if (c.speaker.getState() == DISABLE) {
          c.speaker.setState(ENABLE);
        }
        if (c.lightEffect.getState() == DISABLE) {
          c.lightEffect.setState(ENABLE);
        }
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
      }
      else if (c.codeReader.getTransmittedKey() == keyForCodeReader6) {
        c.informationDisplay.setCurrentSeries(6);
      }
      else{
        c.informationDisplay.setCurrentSeries(0);
      }
    }

  }

  void show(Components & c)
  { 
    c.showTimer.current = millis();
    if ((c.showTimer.current - c.showTimer.showpoint) > c.showTimer.interval) {
      c.showTimer.showpoint = millis();
      c.informationDisplay.display();
      c.powerSwitch.display();
      c.lightEffect.display();
    }

    c.codeReader.display();
  }

  void sound(Components & c) 
  {
    c.powerSwitchStateChange[1] = c.powerSwitch.getState();
    if (c.powerSwitchStateChange[0] != c.powerSwitchStateChange[1]) {
      c.powerSwitchStateChange[0] = c.powerSwitchStateChange[1];

      if (c.powerSwitch.getState() == ON) {
        c.speaker.setCurrent(SOUND_STATION_UP);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }

      if (c.powerSwitch.getState() == OFF) {
        c.speaker.setCurrent(SOUND_STATION_DOWN);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }
    }

    c.codeReaderModeChange[1] = c.codeReader.readMode();
    if (c.codeReaderModeChange[0] != c.codeReaderModeChange[1]) {
      c.codeReaderModeChange[0] = c.codeReaderModeChange[1];

      if (c.codeReader.readMode() == INPUT_MODE) {
        c.speaker.setCurrent(SOUND_DIAL_INPUT);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }

      if (c.codeReader.readMode() == CLEAR_MODE) {
        c.speaker.setCurrent(SOUND_DIAL_CLEAR);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }
    }

    c.codeReaderTransmitChange[1] = c.codeReader.getTransmittedKey();
    if (c.codeReaderTransmitChange[0] != c.codeReaderTransmitChange[1]) {
      c.codeReaderTransmitChange[0] = c.codeReaderTransmitChange[1];
      c.speaker.setCurrent(SOUND_TRANSMIT_BUTTON);
      c.speaker.setRepeat(false);
      c.speaker.setPlayPartly(false);
    }
    
    c.speaker.play(150);
  }
}

#endif