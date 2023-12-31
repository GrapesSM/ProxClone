/*
  PowerControl.h - Library for controlling energy and syncro reader.
*/
#ifndef PowerControl_h
#define PowerControl_h

#include <Arduino.h>
#include "PowerAdjuster.h"
#include "PowerLightIndicator.h"
#include "Battery.h"
#include "LightEffect.h"
#include "Speaker.h"

namespace PowerControl {
  typedef struct {
    PowerAdjuster powerAdjuster;
    PowerLightIndicator powerLightIndicator;
    Battery battery;
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
    unsigned long failurePeriod = 45000;
    int powerLightIndicatorStateChange[2] = {0, 0}; // old, new
    float powerAdjusterSupplyChange[2] = {0, 0}; // old, new
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

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_DEMAND &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.powerAdjuster.setDemand(p.registers[REG_MASTER_BODY] / 10.0);  
      p.registers[REG_SLAVE_CONFIRM] = DONE;   
      p.registers[REG_MASTER_BODY] = 0;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_BATTERY_CHARGING_RATE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {      
      c.battery.setRate(p.registers[REG_MASTER_BODY] / 10.0);
      p.registers[REG_SLAVE_CONFIRM] = DONE;   
      p.registers[REG_MASTER_BODY] = 0;
    }
    
    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_ADJUSTER_STATE] = c.powerAdjuster.getState();
    p.registers[REG_SLAVE_BATTERY_STATE] = c.battery.getState();
    p.registers[REG_SLAVE_POWER_LIGHT_INDICATOR_STATE] = c.powerLightIndicator.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();
    p.registers[REG_SLAVE_LIGHT_EFFECT_STATE] = c.lightEffect.getState();
    p.registers[REG_SLAVE_BATTERY] = c.battery.getValue();
    p.registers[REG_SLAVE_BATTERY_MAX_VALUE] = c.battery.getMaxValue();
    p.registers[REG_SLAVE_SUPPLY] = (int)(c.powerAdjuster.getSupply() * 10);
    p.registers[REG_SLAVE_DEMAND] = (int)(c.powerAdjuster.getDemand() * 10);

    if (c.state == SETUP) {
      c.state = INITIALIZING;
      p.registers[REG_SLAVE_CONFIRM] = DONE;

      c.powerLightIndicator.init();
      c.battery.init();
      c.powerAdjuster.init();
      c.speaker.init();
      c.lightEffect.init();
      
      c.powerLightIndicator.setState(DISABLE);
      
      c.battery.setState(DISABLE);
      c.battery.setRate(12.00);
      c.battery.setMaxValue(13.00);

      c.powerAdjuster.setState(DISABLE);
      c.powerAdjuster.setDemand(p.registers[REG_SLAVE_DEMAND]);
      c.powerAdjuster.setMaxDemand(16.00);
      c.powerAdjuster.setMaxSupply(13.00);

      c.speaker.setState(DISABLE);
      
      c.lightEffect.setState(DISABLE);
      
      c.state = DISABLE;
    }
  }

  void run(Components & c)
  {
    c.powerAdjuster.update();
    c.powerLightIndicator.update();
    c.battery.update();
    c.lightEffect.update();
    c.speaker.update();

    if (c.state == DISABLE) {
      c.powerLightIndicator.setState(DISABLE);
      c.battery.setState(DISABLE);
      c.powerAdjuster.setState(DISABLE);
      c.lightEffect.setState(DISABLE);
      c.speaker.setState(DISABLE);
    }

    if (c.state == RESET) {
      c.state = SETUP;
    }

    if (c.state == ENABLE) {
      if (c.powerAdjuster.getState() == DISABLE) {
        c.powerAdjuster.setState(ENABLE);
      }
      if (c.powerLightIndicator.getState() == DISABLE) {
        c.powerLightIndicator.setState(ENABLE);
      }
      if (c.battery.getState() == DISABLE) {
        c.battery.setState(ENABLE);
      }
      if (c.lightEffect.getState() == DISABLE) {
        c.lightEffect.setState(ENABLE);
      }

      if (c.speaker.getState() == DISABLE) {
        c.speaker.setState(ENABLE);
      }

      c.battery.setDrawRate(c.powerAdjuster.getSupply());

      if (c.powerAdjuster.getState() == UNBALANCED) {
        c.timer.current = millis();
        if (c.timer.start == 0) {
          c.timer.start = millis();
        }

        if ((c.timer.current - c.timer.start) > (c.failurePeriod / 3 * 3)) {
          c.powerLightIndicator.setState(FAILURE);
        } else if ((c.timer.current - c.timer.start) > (c.failurePeriod / 3 * 2)) {
          c.lightEffect.setState(FLASH);
          c.powerLightIndicator.setState(FLASH);
        } else if ((c.timer.current - c.timer.start) > (c.failurePeriod / 3 * 1)) {
          c.powerLightIndicator.setState(FLASH);
        } 
      }

      if (c.powerAdjuster.getState() == BALANCED) {
        c.powerLightIndicator.setState(ENABLE);
        c.lightEffect.setState(ENABLE);
        c.battery.setState(ENABLE);
        c.timer.start = 0;
      }

      if (c.powerLightIndicator.getState() == FAILURE) {
        c.lightEffect.setState(FAILURE);
      }
    }
  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if ((c.showTimer.current - c.showTimer.showpoint) > c.showTimer.interval) {
      c.showTimer.showpoint = millis();
      
      c.powerAdjuster.display();
      c.powerLightIndicator.display();
      c.lightEffect.display();
      c.battery.display();
    }
  }

  void sound(Components & c)
  {
    c.powerAdjusterSupplyChange[1] = c.powerAdjuster.getSupply();
    if (c.powerAdjusterSupplyChange[0] != c.powerAdjusterSupplyChange[1]) {
      c.powerAdjusterSupplyChange[0] = c.powerAdjusterSupplyChange[1];
      c.speaker.setCurrent(SOUND_POWER_ADJUSTER_DIAL);
      c.speaker.setRepeat(false);
      c.speaker.setPlayPartly(true);
    }

    c.powerLightIndicatorStateChange[1] = c.powerLightIndicator.getState();
    if (c.powerLightIndicatorStateChange[0] != c.powerLightIndicatorStateChange[1]) {
      c.powerLightIndicatorStateChange[0] = c.powerLightIndicatorStateChange[1];
      
      if (c.powerLightIndicator.getState() == ENABLE) {
        c.speaker.setCurrent(SOUND_POWER_UP);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }
      
      if (c.powerLightIndicator.getState() == FAILURE) {
        c.speaker.setCurrent(SOUND_POWER_DOWN);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }

      if (c.powerLightIndicator.getState() == FLASH) {
        c.speaker.setCurrent(SOUND_POWER_CRITICAL);
        c.speaker.setRepeat(true);
        c.speaker.setPlayPartly(true);
      }
    }
    
    if (c.speaker.getPlayPartly()) {
      c.speaker.playBytes(1024);
    } else {
      c.speaker.play();
    }
  }
}

#endif