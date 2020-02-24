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

namespace PowerPanel {
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
  } Components;

  void update(Puzzle & p, Components & c) 
  {
    p.registers[REG_POWER_STATE] = ON;
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_ADJUSTER_STATE] = c.powerAdjuster.getState();
    p.registers[REG_SLAVE_BATTERY_STATE] = c.battery.getState();
    p.registers[REG_SLAVE_POWER_LIGHT_INDICATOR_STATE] = c.powerLightIndicator.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();
    p.registers[REG_SLAVE_LIGHT_EFFECT_STATE] = c.lightEffect.getState();
    
    c.battery.setMaxValue(13.00);
    c.powerAdjuster.setMaxSupply(13.00);
    c.powerAdjuster.setMaxDemand(16.00);
    p.registers[REG_SLAVE_SUPPLY] = c.powerAdjuster.getSupply();
    c.powerAdjuster.setDemand(p.registers[REG_SLAVE_DEMAND]);
    // c.lightEffect.setPatternNumber(p.registers[REG_SLAVE_LIGHT_EFFECT_PATTERN_NUMBER]);    
    
    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.powerLightIndicator.setState(ON);
      c.battery.setState(ON);
      c.powerAdjuster.setState(ON);
      c.speaker.setState(ON);
      c.lightEffect.setState(ON);
      c.state = INITIALIZED;
    }
  }

  void run(Components & c)
  {
    c.timer.current = millis();

    if (c.state == INITIALIZED) {
      c.state = STANDBY;
    }

    if (c.state == STANDBY || c.state == FAILURE) {
      c.powerAdjuster.update();
      int batteryLevel = map(c.powerAdjuster.getSupply(), 0.0, c.powerAdjuster.getMaxSupply(), 0, 100);
      c.battery.setValue(batteryLevel);
      c.battery.update();
      c.lightEffect.update();

      if (c.powerAdjuster.getState() == UNBALANCED) {
        if (c.timer.start == 0) {
          c.timer.start = millis();
        }

        if ((c.timer.current - c.timer.start) > FAILURE_PERIOD_3) {
          c.speaker.setState(OFF);
          c.powerLightIndicator.setState(OFF);
          c.lightEffect.setState(FAILURE);
          c.powerAdjuster.setState(RESET);
          c.battery.setState(OFF);
          c.state = FAILURE;
        } else if ((c.timer.current - c.timer.start) > FAILURE_PERIOD_2) {
          c.speaker.setState(ALARM);
          c.lightEffect.setState(FLASH);
          c.powerLightIndicator.setState(FLASH);
        } else if ((c.timer.current - c.timer.start) > FAILURE_PERIOD_1) {
          c.powerLightIndicator.setState(FLASH);
        } 
      }

      if (c.powerAdjuster.getState() == BALANCED) {
        c.powerLightIndicator.setState(ON);
        c.speaker.setState(OFF);
        c.lightEffect.setState(ON);
        c.battery.setState(ON);
        c.timer.start = 0;
      }
    }
  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if ((c.showTimer.current - c.showTimer.showpoint) > c.showTimer.interval) {
      c.powerAdjuster.display();
      c.powerLightIndicator.display();
      c.lightEffect.display();
      // c.battery.display();
      c.showTimer.showpoint = millis();
    }

    c.speaker.play();
  }
}

#endif