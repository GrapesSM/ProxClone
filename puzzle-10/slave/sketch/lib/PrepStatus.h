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
    boolean batteryMatrixFlag = false;
    boolean energySuppFlag = false;
    boolean generatorFlag = false;
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

    if (p.registers[REG_MASTER_COMMAND] == CMD_START_TIMER &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
          c.syncroReader.setState(START_TIMER);
          p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.syncroReader.setState(SOLVED);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE_PS_SYNCRO_KEY &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.syncroReader.setState(ENABLE);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_BATTERY_MATRIX_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.batteryMatrixFlag = true;
      c.batteryMatrix.setState(SOLVED);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_ENERGY_SUPP_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.energySuppFlag = true;
      c.energySupp.setState(SOLVED);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_GENERATOR_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.generatorFlag = true;
      c.generator.setState(SOLVED);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }
    
    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_BATTERY_MATRIX_STATE] = c.batteryMatrix.getState();
    p.registers[REG_SLAVE_ENERGY_SUPP_STATE] = c.energySupp.getState();
    p.registers[REG_SLAVE_GENERATOR_STATE] = c.generator.getState();
    p.registers[REG_SLAVE_SYNCRO_READER_STATE] = c.syncroReader.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();
    p.registers[REG_SLAVE_LIGHT_EFFECT_STATE] = c.lightEffect.getState();
    p.registers[REG_SLAVE_SYNCRO_READER_INPUT_KEY] = c.syncroReader.getInputKey();
    // Serial.println(c.syncroReader.getInputKey());
    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.powerSwitch.setState(DISABLE);
      c.batteryMatrix.setState(DISABLE);
      c.energySupp.setState(DISABLE);
      c.generator.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = INITIALIZED;
    }
  }

  void run(Components & c) 
  {
    if (c.state == INITIALIZED) {
      c.state = ENABLE;
    }
    
    c.batteryMatrix.update();
    c.energySupp.update();
    c.generator.update();
    c.syncroReader.update();
    c.powerSwitch.update();
    c.lightEffect.update();    

    if (c.state == ENABLE) {
      if (c.powerSwitch.getState() == DISABLE) {
        c.powerSwitch.setState(ENABLE);
      }

      if (c.powerSwitch.getState() == OFF) {
        c.batteryMatrix.setState(DISABLE);
        c.energySupp.setState(DISABLE);
        c.generator.setState(DISABLE);
        c.syncroReader.setState(DISABLE);
        c.lightEffect.setState(DISABLE);
        if (c.speaker.getNumber() != SOUND_POWER_DOWN) {
          c.speaker.addToPlay(SOUND_POWER_DOWN);
        }
      } 

      if (c.powerSwitch.getState() == ON) {
        if (c.batteryMatrix.getState() == DISABLE) 
          c.batteryMatrix.setState(ENABLE);
        if (c.energySupp.getState() == DISABLE) 
          c.energySupp.setState(ENABLE);
        if (c.generator.getState() == DISABLE) 
          c.generator.setState(ENABLE);
        if (c.speaker.getNumber() == SOUND_POWER_DOWN) {
          c.speaker.addToPlay(SOUND_POWER_UP);
        }
        if (c.lightEffect.getState() == DISABLE) 
          c.lightEffect.setState(ENABLE);
        
        if (c.syncroReader.getSyncroKeyState() && c.speaker.getNumber() != SOUND_KEY_INSERT) {
          c.speaker.addToPlay(SOUND_KEY_INSERT);
        }

        if (! c.syncroReader.getSyncroKeyState() && c.speaker.getNumber() == SOUND_KEY_INSERT) {
          c.speaker.setNumber(SOUND_POWER_UP);
        }
      }


      if(
        c.batteryMatrix.getState() == SOLVED &&
        c.energySupp.getState() == SOLVED &&
        c.generator.getState() == SOLVED
      )
      {
        c.batteryMatrix.switchToGreen();
        c.energySupp.switchToGreen();
        c.generator.switchToGreen();
        if (c.syncroReader.getState() == DISABLE) {
          c.syncroReader.setState(ENABLE);
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

    if (c.state == DISABLE) {
      c.powerSwitch.setState(DISABLE);
      c.batteryMatrix.setState(DISABLE);
      c.energySupp.setState(DISABLE);
      c.generator.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
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

    c.speaker.play();
  }
}

#endif