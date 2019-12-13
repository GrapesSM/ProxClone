/*
  Safeomatic.h - Library for ____.
*/
#ifndef Safeomatic_h
#define Safeomatic_h

#include <Arduino.h>
#include "CombinationReader.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace Datamatic {
  typedef struct {
    CombinationReader combinationReader;
    PowerSwitch powerSwitch;
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