/*
  PrepStatus.h - Library for controlling energy and syncro reader.
*/
#ifndef PrepStatus_h
#define PrepStatus_h

#include <Arduino.h>
#include "BatteryMatrix.h"
#include "EnergySupp.h"
#include "LightEffect.h"
#include "Generator.h"
#include "SyncroReader.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace PrepStatus {
  typedef struct {
    BatteryMatrix batteryMatrix;
    EnergySupp energySupp;
    LightEffect lightEffect;
    Generator generator;
    SyncroReader syncroReader;
    PowerSwitch powerSwitch;
    Speaker speaker;
    STATE state;
    boolean flag;
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
  } Components;

  void update(Puzzle & p, Components & c) 
  {
    p.registers[REG_POWER_STATE] = c.powerSwitch.isSwitchOn() ? ON : OFF;
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_BATTERY_MATRIX_STATE] = c.batteryMatrix.getState();
    p.registers[REG_SLAVE_ENERGY_SUPP_STATE] = c.energySupp.getState();
    p.registers[REG_SLAVE_GENERATOR_STATE] = c.generator.getState();
    p.registers[REG_SLAVE_SYNCRO_READER_STATE] = c.syncroReader.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();
    p.registers[REG_SLAVE_LIGHT_EFFECT_STATE] = c.lightEffect.getState();
    p.registers[REG_SLAVE_SYNCRO_READER_INPUT_KEY] = c.syncroReader.getInputKey();
    if (p.registers[REG_MASTER_COMMAND] == CMD_START_TIMER &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      if (c.syncroReader.getState() == DONE || c.syncroReader.getState() == ENABLE) {
        c.syncroReader.setState(START_TIMER);
        p.registers[REG_MASTER_CONFIRM] = DONE;
      }
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_SYNCRONIZED &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      c.syncroReader.setState(SYNCRONIZED);
      p.registers[REG_MASTER_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_BATTERY_MATRIX_SOLVED &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      c.batteryMatrix.setState(SOLVED);
      p.registers[REG_MASTER_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_ENERGY_SUPP_SOLVED &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      c.energySupp.setState(SOLVED);
      p.registers[REG_MASTER_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_GENERATOR_SOLVED &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      c.generator.setState(SOLVED);
      p.registers[REG_MASTER_CONFIRM] = DONE;
    }
    
    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.batteryMatrix.setState(DISABLE);
      c.energySupp.setState(DISABLE);
      c.generator.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
      c.state = INITIALIZED;
    }
  }

  void run(Components & c) 
  {
    if (c.state == INITIALIZED) {
      c.state = UNSOLVED;
    }

    c.powerSwitch.update();
    c.lightEffect.update();

    if (c.powerSwitch.getState() == OFF) {
      c.batteryMatrix.setState(DISABLE);
      c.energySupp.setState(DISABLE);
      c.generator.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
    } 
    
    if (c.powerSwitch.getState() == ON) {
      if (c.batteryMatrix.getState() == DISABLE) 
        c.batteryMatrix.setState(ENABLE);
      if (c.energySupp.getState() == DISABLE) 
        c.energySupp.setState(ENABLE);
      if (c.generator.getState() == DISABLE) 
        c.generator.setState(ENABLE);
      if (c.speaker.getState() == DISABLE) 
        c.speaker.setState(ENABLE);
      if (c.lightEffect.getState() == DISABLE) 
        c.lightEffect.setState(ENABLE);
    }

    if (c.state == UNSOLVED) {
      c.batteryMatrix.update();
      c.energySupp.update();
      c.generator.update();
      c.syncroReader.update();

      if (
        c.batteryMatrix.getState() == SOLVED &&
        c.energySupp.getState() == SOLVED &&
        c.generator.getState() == SOLVED
      ) {
        c.batteryMatrix.switchToGreen();
        c.energySupp.switchToGreen();
        c.generator.switchToGreen();
        if (c.syncroReader.getState() == DISABLE)
          c.syncroReader.setState(ENABLE);
      } 

      if (
        c.batteryMatrix.getState() == SOLVED &&
        c.energySupp.getState() == SOLVED &&
        c.generator.getState() == SOLVED &&
        c.syncroReader.getState() == SYNCRONIZED
      ) {
        c.state = SOLVED; 
      }
    }  


    if (c.state == SOLVED) {
      Serial.println("SOLVED");
    }
  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if ((c.showTimer.current - c.showTimer.showpoint) > c.showTimer.interval) {
      c.showTimer.showpoint = millis();
    
      c.powerSwitch.display();
      c.lightEffect.display();
      c.syncroReader.display();
    }
  }
}

#endif