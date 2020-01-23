/*
  LifeSupport.h - Library for _______.
*/
#ifndef LifeSupport_h
#define LifeSupport_h

#include <Arduino.h>
#include "ExternalVent.h"
#include "AirSupplyPump.h"
#include "AirPressureStatus.h"
#include "PowerSwitch.h"
#include "LightEffect.h"
#include "Speaker.h"

namespace LifeSupport {
  typedef struct {
    ExternalVent externalVent;
    AirSupplyPump airSupplyPump;
    AirPressureStatus airPressureStatus;
    PowerSwitch powerSwitch;
    LightEffect lightEffect;
    Speaker speaker;
    STATE state;
  } Components;

  void run(Components c) 
  {
    if (c.powerSwitch.isSwitchOff()) {
      c.powerSwitch.setLightOff();
      c.externalVent.disable();
      c.airSupplyPump.disable();
      c.airPressureStatus.disable();
      c.lightEffect.disable();
    } else {
      c.powerSwitch.setLightOn();
      c.externalVent.enable();
      c.airSupplyPump.enable();
      c.airPressureStatus.enable();
      c.lightEffect.enable();
    }

    if (! c.externalVent.isDisabled()) {
      c.externalVent.update();
    } 

    if (! c.airSupplyPump.isDisabled()) {
      c.airSupplyPump.update();
    }

    if (! c.airPressureStatus.isDisabled()) {
      c.airPressureStatus.update();
    }
  }

  void show(Components c)
  {
    c.airPressureStatus.display();
    c.powerSwitch.display();
    c.lightEffect.display();
  }
}

#endif