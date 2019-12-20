/*
  ShipPrepAux.h - Library for controlling energy and syncro reader.
*/
#ifndef ShipPrepAux_h
#define ShipPrepAux_h

#include <Arduino.h>
#include "BatteryMatrix.h"
#include "EnergySupp.h"
#include "LightEffect.h"
#include "Generator.h"
#include "SyncroReader.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace ShipPrepAux {
  typedef struct {
    BatteryMatrix batteryMatrix;
    EnergySupp energySupp;
    LightEffect lightEffect;
    Generator generator;
    SyncroReader syncroReader;
    PowerSwitch powerSwitch;
    Speaker speaker;
    STATE state;
  } Components;

  void run(Components c) 
  {
    if (c.powerSwitch.isSwitchOff()) {
      c.powerSwitch.setLightOff();
      c.batteryMatrix.disable();
      c.energySupp.disable();
      c.generator.disable();
      c.syncroReader.disable();
      c.lightEffect.disable();
    } else {
      c.powerSwitch.setLightOn();
      c.batteryMatrix.enable();
      c.energySupp.enable();
      c.generator.enable();
      c.syncroReader.enable();
      c.lightEffect.enable();
    }

    if (! c.syncroReader.isDisabled()) {
      c.syncroReader.update();
    }

    if (! c.batteryMatrix.isDisabled()) {
      c.batteryMatrix.update();
    }

    if (! c.energySupp.isDisabled()) {
      c.energySupp.update();
    }

    if (! c.generator.isDisabled()) {
      c.generator.update();
    }
  }

  void show(Components c)
  {
    c.powerSwitch.display();
    c.batteryMatrix.display();
    c.energySupp.display();
    c.generator.display();
    c.syncroReader.display();
    c.lightEffect.display();
  }
}

#endif