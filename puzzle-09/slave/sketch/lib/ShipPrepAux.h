/*
  ShipPrepAux.h - Library for controlling energy and syncro reader.
*/
#ifndef ShipPrepAux_h
#define ShipPrepAux_h

#include <Arduino.h>
#include "BatteryMatrix.h"
#include "Generator.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace ShipPrepAux {
  typedef struct {
    BatteryMatrix batteryMatrix;
    Generator generator;
    PowerSwitch powerSwitch;
    Speaker speaker;
    STATE state;
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
    int powerSwitchStateChange[2] = {0, 0}; // old, new
    int batteryMatrixSwitchStateChange[2] = {0, 0}; // old, new
    int generatorSwitchStateChange[2] = {0, 0}; // old, new
  } Components;

  void update(Puzzle & p, Components & c)
  {

    if (p.registers[REG_MASTER_SP_COMMAND] == CMD_ENABLE &&
        p.registers[REG_SLAVE_SP_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_SP_CONFIRM] = DONE;
      c.state = ENABLE;
    }

    if (p.registers[REG_MASTER_SP_COMMAND] == CMD_DISABLE &&
        p.registers[REG_SLAVE_SP_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_SP_CONFIRM] = DONE;
      c.state = DISABLE;
    }

    if (p.registers[REG_MASTER_SP_COMMAND] == CMD_RESET &&
        p.registers[REG_SLAVE_SP_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_SP_CONFIRM] = DONE;
      c.state = RESET;
    }

    if (p.registers[REG_MASTER_SP_COMMAND] == CMD_SET_SOLVED &&
        p.registers[REG_SLAVE_SP_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_SP_CONFIRM] = DONE;
      c.state = SOLVED;
    }

    if (p.registers[REG_MASTER_SP_COMMAND] == CMD_SET_DS_BATTERY_MATRIX_SOLVED &&
        p.registers[REG_SLAVE_SP_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_SP_CONFIRM] = DONE;
      c.batteryMatrix.setState(SOLVED);
    }

    if (p.registers[REG_MASTER_SP_COMMAND] == CMD_SET_DS_GENERATOR_SOLVED &&
        p.registers[REG_SLAVE_SP_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_SP_CONFIRM] = DONE;
      c.generator.setState(SOLVED);
    }

    p.registers[REG_SLAVE_SP_MILLIS] = millis();
    p.registers[REG_SLAVE_SP_STATE] = c.state;
    p.registers[REG_SLAVE_SP_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_SP_BATTERY_MATRIX_STATE] = c.batteryMatrix.getState();
    p.registers[REG_SLAVE_SP_GENERATOR_STATE] = c.generator.getState();
    p.registers[REG_SLAVE_SP_SPEAKER_STATE] = c.speaker.getState();

    if (c.state == SETUP) {
      c.state = INITIALIZING;

      c.powerSwitch.init();
      c.batteryMatrix.init();
      c.generator.init();
      c.speaker.init();

      c.powerSwitch.setState(DISABLE);
      c.batteryMatrix.setState(DISABLE);
      c.generator.setState(DISABLE);
      c.speaker.setState(DISABLE);
      p.registers[REG_SLAVE_SP_CONFIRM] = DONE;
      c.state = ENABLE;
    }
  }

  void run(Components &c) 
  {
    c.powerSwitch.update();
    c.batteryMatrix.update();
    c.generator.update();
    c.speaker.update();

    if (c.powerSwitch.getState() == DISABLE) {
      c.powerSwitch.setState(ENABLE);
    }

    if (c.state == DISABLE) {
      c.powerSwitch.setLightOff();
      c.batteryMatrix.setState(DISABLE);
      c.generator.setState(DISABLE);
      c.speaker.setState(DISABLE);
    }

    if (c.state == RESET) {
      c.state = SETUP;
    }

    if (c.state == ENABLE) {

      if (c.powerSwitch.getState() == OFF) {
        c.batteryMatrix.setState(DISABLE);
        c.generator.setState(DISABLE);
      } 

      if (c.powerSwitch.getState() == ON) {
        if (c.batteryMatrix.getState() == DISABLE)
          c.batteryMatrix.setState(ENABLE);
        if (c.generator.getState() == DISABLE) 
          c.generator.setState(ENABLE);
        if (c.speaker.getState() == DISABLE)
          c.speaker.setState(ENABLE);
      }

      if (c.batteryMatrix.getInputKey() == keyForBatteryMatrix && c.batteryMatrix.getState() == ENABLE) {
        c.batteryMatrix.setState(SOLVED);
      }

      if (c.generator.getInputKey() == keyForGenerator && c.generator.getState() == ENABLE) {
        c.generator.setState(SOLVED);
      }  
      if (c.generator.getState() == SOLVED && c.batteryMatrix.getState() == SOLVED) {
        c.state = SOLVED;
      }      
    }

    if (c.state == SOLVED) {
      if (c.powerSwitch.getState() == OFF) {
        c.batteryMatrix.setState(DISABLE);
        c.generator.setState(DISABLE);
      }
      
      if (c.powerSwitch.getState() == ON) {
        c.batteryMatrix.setState(SOLVED);
        c.generator.setState(SOLVED);
      }
    }

  }

  void show(Components &c)
  {
    c.showTimer.current = millis();
    if (c.showTimer.current - c.showTimer.showpoint > c.showTimer.interval) {
      c.showTimer.showpoint = millis();

      // code here runs every interval time
      // c.powerSwitch.display();
    }
  }

  void sound(Components &c) {

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

    c.batteryMatrixSwitchStateChange[1] = c.batteryMatrix.getSwitchState();
    if (c.batteryMatrixSwitchStateChange[0] != c.batteryMatrixSwitchStateChange[1]) {
      c.batteryMatrixSwitchStateChange[0] = c.batteryMatrixSwitchStateChange[1];
      c.speaker.setCurrent(SOUND_SWITCH);
      c.speaker.setRepeat(false);
      c.speaker.setPlayPartly(true);
    }

    c.generatorSwitchStateChange[1] = c.generator.getSwitchState();
    if (c.generatorSwitchStateChange[0] != c.generatorSwitchStateChange[1]) {
      c.generatorSwitchStateChange[0] = c.generatorSwitchStateChange[1];
      c.speaker.setCurrent(SOUND_SWITCH);
      c.speaker.setRepeat(false);
      c.speaker.setPlayPartly(true);
    }

    c.speaker.play();
  }
}

#endif