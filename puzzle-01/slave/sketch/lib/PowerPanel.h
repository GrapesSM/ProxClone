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

#include <EventManager.h>

namespace PowerPanel {
  typedef struct {
    PowerAdjuster powerAdjuster;
    PowerLightIndicator powerLightIndicator;
    Battery battery;
    LightEffect lightEffect;
    Speaker speaker;
    EventManager eventManager;
    STATE state;
    float currentDemand = 0.0;
    float currentSupply = 0.0;
    float maxSupply = 7.0;
    struct Timer {
      unsigned long start = 0;
      unsigned long end = 0;
      unsigned long current = 0;
    } timer;
  } Components;

  void update(Puzzle & p, Components & c) 
  {
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_ADJUSTER_STATE] = c.powerAdjuster.getState();
    p.registers[REG_SLAVE_BATTERY_STATE] = c.battery.getState();
    p.registers[REG_SLAVE_POWER_LIGHT_INDICATOR_STATE] = c.powerLightIndicator.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();
    p.registers[REG_SLAVE_LIGHT_EFFECT_STATE] = c.lightEffect.getState();
    
    // c.battery.setValue(p.registers[REG_SLAVE_BATTERY]);
    p.registers[REG_SLAVE_SUPPLY] = c.powerAdjuster.getSupplyValue();
    c.powerAdjuster.setDemandValue(p.registers[REG_SLAVE_DEMAND]);
    // c.lightEffect.setPatternNumber(p.registers[REG_SLAVE_LIGHT_EFFECT_PATTERN_NUMBER]);    
    
    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.powerLightIndicator.setState(FLASH);
      // c.battery.init();
      // c.powerAdjuster.init();
      // c.speaker.init();
      // c.lightEffect.init();
      c.state = INITIALIZED;
    }
  }

  void run(Components & c)
  {
    c.timer.current = millis();

    if (c.state == INITIALIZED) {
      c.state = STANDBY;
    }

    if (c.state == STANDBY) {
      c.powerAdjuster.update();
      c.battery.setValue(c.powerAdjuster.getSupplyValue());

      if (c.powerAdjuster.getState() != BALANCED) {
        // if (c.timer.start == 0) {
        //   c.timer.start = millis();
        // }

        // if ((c.timer.current - c.timer.start) > 9000) {
        //   c.speaker.setAlarmOff();
        //   c.powerLightIndicator.setOff();
        //   c.lightEffect.setOff();
        //   c.powerAdjuster.setSupplyValue(0);
        //   c.powerAdjuster.setDemandValue(0);
        //   c.battery.setValue(0);
        //   c.state = FAILURE;
        // } else if ((c.timer.current - c.timer.start) > 6000) {
        //   c.speaker.setAlarmOn();
        //   c.lightEffect.setFlashOn();
        //   c.powerLightIndicator.setFlashOn();
        //   c.state = STANDBY;
        // } else if ((c.timer.current - c.timer.start) > 3000) {
        //   c.powerLightIndicator.setFlashOn();
        //   c.state = STANDBY;
        // } 
      }

      if (c.powerAdjuster.getState() == BALANCED) {
        // c.powerLightIndicator.setOn();
        // c.timer.start = 0;
      }
    }
  }

  void show(Components & c)
  {
    c.powerAdjuster.display();
    c.powerLightIndicator.display();
    // c.battery.display();
    c.lightEffect.display();
  }

  void onPowerChange(int event, Components & c) 
  {
    Serial.println("Power Switch");
  }
}

#endif