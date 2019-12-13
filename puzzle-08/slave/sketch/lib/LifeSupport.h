/*
  LifeSupport.h - Library for controlling energy and syncro reader.
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
    // TO-DO:
  }

  void show(Components c)
  {
    // TO-DO:
  }
}

#endif