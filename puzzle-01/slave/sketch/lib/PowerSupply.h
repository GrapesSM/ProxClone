/*
  PowerSupply.h - Library for controlling energy and syncro reader.
*/
#ifndef PowerSupply_h
#define PowerSupply_h

#include <Arduino.h>
#include "PowerAdjuster.h"
#include "SyncroReader.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace PowerSupply {
  typedef struct {
    PowerControl powerControl;
    PowerIndicator powerIndicator;
    LightEffect lightEffect;
    Generator generator;
    Speaker speaker;
    STATE state;
  } Components;

  void run(Components c) 
  {
    
  }

  void show(Components c)
  {
    
  }
}

#endif