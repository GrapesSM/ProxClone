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
    } showTimer, showTimer1;
    boolean batteryMatrixFlag = false;
    boolean energySuppFlag = false;
    boolean generatorFlag = false;
    int powerSwitchStateChange[2] = {0, 0}; // old, new
    int syncroKeyStateChange[2] = {0, 0}; // old, new
    int syncroCountdownStateChange[2] = {0, 0}; // old, new
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

    if (p.registers[REG_MASTER_COMMAND] == CMD_START_TIMER &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      if (p.registers[REG_MASTER_BODY] > 0) {
        vTaskDelay(p.registers[REG_MASTER_BODY]);
        p.registers[REG_MASTER_BODY] = 0;
      }
      c.syncroReader.startTimer();
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = SOLVED;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE_PS_SYNCRO_READER &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.syncroReader.setState(DONE);
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

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_PS_SYNCRO_READER_SYNCRONIZED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.syncroReader.setState(SYNCRONIZED);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_PS_SYNCRO_READER_WRONG_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.syncroReader.setState(FLASH);
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
    
    if (c.state == SETUP) {
      c.state = INITIALIZING;

      c.showTimer1.interval = 50;

      c.powerSwitch.init();
      c.batteryMatrix.init();
      c.energySupp.init();
      c.generator.init();
      c.syncroReader.init();
      c.speaker.init();
      c.lightEffect.init();

      c.powerSwitch.setState(DISABLE);
      c.batteryMatrix.setState(DISABLE);
      c.energySupp.setState(DISABLE);
      c.generator.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = DISABLE;
    }
  }

  void run(Components & c) 
  {
    c.batteryMatrix.update();
    c.energySupp.update();
    c.generator.update();
    c.syncroReader.update();
    c.powerSwitch.update();
    c.lightEffect.update();    

    if (c.powerSwitch.getState() == DISABLE) {
      c.powerSwitch.setState(ENABLE);
    }

    if (c.state == DISABLE) {
      c.powerSwitch.setLightOff();
      c.batteryMatrix.setState(DISABLE);
      c.energySupp.setState(DISABLE);
      c.generator.setState(DISABLE);
      c.syncroReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
    }

    if (c.state == RESET) {
      c.state = SETUP;
    }

    if (c.state == ENABLE) {
      if (c.powerSwitch.getState() == OFF) {
        c.batteryMatrix.setState(DISABLE);
        c.energySupp.setState(DISABLE);
        c.generator.setState(DISABLE);
        c.syncroReader.setState(DISABLE);
        c.lightEffect.setState(DISABLE);
      } 

      if (c.powerSwitch.getState() == ON) {
        if (c.batteryMatrix.getState() == DISABLE) {
          c.batteryMatrix.setState(ENABLE);
        }
        if (c.energySupp.getState() == DISABLE) {
          c.energySupp.setState(ENABLE);
        }
        if (c.generator.getState() == DISABLE) {
          c.generator.setState(ENABLE);
        }
        if (c.lightEffect.getState() == DISABLE) {
          c.lightEffect.setState(ENABLE);
        }
        if (c.speaker.getState() == DISABLE) {
          c.speaker.setState(ENABLE);
        }
      }

      if (
        c.batteryMatrix.getState() == SOLVED &&
        c.energySupp.getState() == SOLVED &&
        c.generator.getState() == SOLVED
      )
      {
        c.batteryMatrix.switchToGreen();
        c.energySupp.switchToGreen();
        c.generator.switchToGreen();
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
      if (c.powerSwitch.getState() == OFF) {
        c.batteryMatrix.setState(DISABLE);
        c.energySupp.setState(DISABLE);
        c.generator.setState(DISABLE);
        c.syncroReader.setState(DISABLE);
        c.lightEffect.setState(DISABLE);
      } 

      if (c.powerSwitch.getState() == ON) {
        c.batteryMatrix.setState(SOLVED);
        c.energySupp.setState(SOLVED);
        c.generator.setState(SOLVED);
        c.syncroReader.setState(SYNCRONIZED);
        c.lightEffect.setState(SOLVED);

        if (
          c.batteryMatrix.getState() == SOLVED &&
          c.energySupp.getState() == SOLVED &&
          c.generator.getState() == SOLVED
        )
        {
          c.batteryMatrix.switchToGreen();
          c.energySupp.switchToGreen();
          c.generator.switchToGreen();
        }
      }
    }
  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if ((c.showTimer.current - c.showTimer.showpoint) > c.showTimer.interval) {
      c.showTimer.showpoint = millis();
    
      c.lightEffect.display();
      c.syncroReader.display();
    }
    
    c.showTimer1.current = millis();
    if ((c.showTimer1.current - c.showTimer1.showpoint) > c.showTimer1.interval) {
      c.showTimer1.showpoint = millis();

      c.powerSwitch.display();
    }

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

    c.syncroKeyStateChange[1] = c.syncroReader.getSyncroKeyState();
    if (c.syncroKeyStateChange[0] != c.syncroKeyStateChange[1]) {
      c.syncroKeyStateChange[0] = c.syncroKeyStateChange[1];
      c.speaker.setCurrent(SOUND_KEY_SWITCH);
      c.speaker.setRepeat(false);
      c.speaker.setPlayPartly(false);
    }

    c.syncroCountdownStateChange[1] = c.syncroReader.getSyncroCountdownState();
    if (c.syncroCountdownStateChange[0] != c.syncroCountdownStateChange[1]) {
      c.syncroCountdownStateChange[0] = c.syncroCountdownStateChange[1];
      c.speaker.setCurrent(SOUND_COUNTDOWN_BEEP);
      c.speaker.setRepeat(false);
      c.speaker.setPlayPartly(false);
    }

    c.speaker.play(200);
  }
}

#endif