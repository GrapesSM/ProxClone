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
    p.registers[REG_SLAVE_TIME] = millis();
    p.registers[REG_SLAVE_SYNCRO_READER_INPUT_KEY] = c.syncroReader.getInputKey();
    p.registers[REG_SLAVE_SYNCRO_READER_LAST_REFRESH_TIME] = c.syncroReader.getLastRefreshTime();
    if (p.registers[REG_MASTER_COMMAND] == CMD_START_TIMER) {
      if (c.syncroReader.getState() == DONE) {
        c.syncroReader.startTimer();
        p.registers[REG_MASTER_CONFIRM] = DONE;
      }
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

    if (c.powerSwitch.getState() == OFF) {
      c.batteryMatrix.setState(DISABLE);
      c.energySupp.setState(DISABLE);
      c.generator.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
    } 
    
    if (c.powerSwitch.getState() == ON) {
      c.batteryMatrix.setState(ENABLE);
      c.energySupp.setState(ENABLE);
      c.generator.setState(ENABLE);
      c.speaker.setState(ENABLE);
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
        c.syncroReader.setState(ENABLE);
      } else {
        if (c.batteryMatrix.getState() == SOLVED) {
          c.batteryMatrix.switchToYellow();
        } else {
          c.batteryMatrix.switchToRed();
        }

        if (c.energySupp.getState() == SOLVED) {
          c.energySupp.switchToYellow();
        } else {
          c.energySupp.switchToRed();
        }

        if (c.generator.getState() == SOLVED) {
          c.generator.switchToYellow();
        } else {
          c.generator.switchToRed();
        }
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

    if(! c.syncroReader.isSynchronized()){
      c.syncroReader.update();
      if(c.syncroReader.getInputKey() == 1){
        c.syncroReader.setSynchronized();
      }
    }
    else{
      c.syncroReader.setSynchronized();
    }
  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if ((c.showTimer.current - c.showTimer.showpoint) > c.showTimer.interval) {
      c.showTimer.showpoint = millis();
    
      c.powerSwitch.display();
      c.lightEffect.display();
    }
    c.syncroReader.display();
  }
}

#endif