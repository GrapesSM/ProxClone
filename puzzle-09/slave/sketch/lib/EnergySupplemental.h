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
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval;
    } showTimer, showTimer1;
  } Components;

  void update(Puzzle & p, Components & c)
  {
    if (p.registers[REG_MASTER_ES_COMMAND] == CMD_ENABLE &&
        p.registers[REG_SLAVE_ES_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      c.state = ENABLE;
    }

    if (p.registers[REG_MASTER_ES_COMMAND] == CMD_DISABLE &&
        p.registers[REG_SLAVE_ES_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      c.state = DISABLE;
    }

    if (p.registers[REG_MASTER_ES_COMMAND] == CMD_RESET &&
        p.registers[REG_SLAVE_ES_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      c.state = RESET;
    }

    if (p.registers[REG_MASTER_ES_COMMAND] == CMD_SET_SOLVED &&
        p.registers[REG_SLAVE_ES_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      c.state = SOLVED;
    }

    if (p.registers[REG_MASTER_ES_COMMAND] == CMD_SET_DS_ENERGY_SUPP_SOLVED &&
        p.registers[REG_SLAVE_ES_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      c.state = SOLVED;
    }

    if (p.registers[REG_MASTER_ES_COMMAND] == CMD_START_TIMER &&
        p.registers[REG_SLAVE_ES_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      if (p.registers[REG_MASTER_ES_BODY] > 0) {
        vTaskDelay(p.registers[REG_MASTER_ES_BODY]);
        p.registers[REG_MASTER_ES_BODY] = 0;
      }
      c.syncroReader.startTimer();
    }

    if (p.registers[REG_MASTER_ES_COMMAND] == CMD_SET_DS_SYNCRO_READER_SYNCRONIZED &&
        p.registers[REG_SLAVE_ES_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      c.syncroReader.setState(SYNCRONIZED);
    }

    if (p.registers[REG_MASTER_ES_COMMAND] == CMD_SET_DS_SYNCRO_READER_WRONG_SOLVED &&
        p.registers[REG_SLAVE_ES_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      c.syncroReader.setState(FLASH);
    }

    if (p.registers[REG_MASTER_ES_COMMAND] == CMD_ENABLE_DS_SYNCRO_READER &&
        p.registers[REG_SLAVE_ES_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;
      c.syncroReader.setState(DONE);
    }
    
    p.registers[REG_SLAVE_ES_MILLIS] = millis();
    p.registers[REG_SLAVE_ES_STATE] = c.state;
    p.registers[REG_SLAVE_ES_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_ES_POWER_ADJUSTER_STATE] = c.powerAdjuster.getState();
    p.registers[REG_SLAVE_ES_SYNCRO_READER_STATE] = c.syncroReader.getState();
    p.registers[REG_SLAVE_ES_SPEAKER_STATE] = c.speaker.getState();
    p.registers[REG_SLAVE_ES_SYNCRO_READER_INPUT_KEY] = c.syncroReader.getInputKey();

    if (c.state == SETUP) {
      c.state = INITIALIZING;

      c.powerSwitch.init();
      c.syncroReader.init();
      c.powerAdjuster.init();
      c.speaker.init();

      c.powerSwitch.setState(DISABLE);
      c.powerAdjuster.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      p.registers[REG_SLAVE_ES_CONFIRM] = DONE;

      c.showTimer.interval = 200;
      c.showTimer1.interval = 50;

      c.state = DISABLE;
    }
  }
  void run(Components &c) 
  {
    c.powerSwitch.update();
    c.powerAdjuster.update();
    c.syncroReader.update();
    c.speaker.update();

    if (c.powerSwitch.getState() == DISABLE) {
      c.powerSwitch.setState(ENABLE);
    }

    if (c.state == DISABLE) {
      c.powerSwitch.setLightOff();
      c.powerAdjuster.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
    }

    if (c.state == RESET) {
      c.state = SETUP;
    }

    if (c.state == ENABLE) {

      if (c.powerSwitch.getState() == OFF) {
        c.powerAdjuster.setState(DISABLE);
        c.syncroReader.setState(DISABLE);
      } 

      if (c.powerSwitch.getState() == ON)
      {
        if (c.powerAdjuster.getState() == DISABLE) 
          c.powerAdjuster.setState(ENABLE);
      }

      if (c.powerAdjuster.getInputKey() == keyForPowerAdjuster && c.powerAdjuster.getState() == ENABLE) {
        c.powerAdjuster.setState(SOLVED);
      }

      if (c.powerAdjuster.getState() == SOLVED && c.syncroReader.getState() == SYNCRONIZED) {
        c.state = SOLVED;
      }
    }

    if (c.state == SOLVED) {
      if (c.powerSwitch.getState() == OFF) {
        c.powerAdjuster.setState(DISABLE);
        c.syncroReader.setState(DISABLE);
      } 

      if (c.powerSwitch.getState() == ON) {
        c.powerAdjuster.setState(SOLVED);
        c.syncroReader.setState(SYNCRONIZED);
      }
    }
  }

  void show(Components &c)
  {
    c.showTimer.current = millis();
    if (c.showTimer.current - c.showTimer.showpoint > c.showTimer.interval) {
      c.showTimer.showpoint = millis();

      // code here runs every interval time
      c.powerAdjuster.display();
      // c.syncroReader.display();
    }

    c.showTimer1.current = millis();
    if (c.showTimer1.current - c.showTimer1.showpoint > c.showTimer1.interval) {
      c.showTimer1.showpoint = millis();

      // code here runs every interval time (i.e. every 50ms)
      c.powerSwitch.display();
    }

    c.speaker.play();
  }
}

#endif