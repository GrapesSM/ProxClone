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
    // TO-DO:
    // Start State
    // End State
    // Winning Indication
    // Connections to other puzzles
    // Mandatory clues/dependencies
    // Optional clues
    // How to solve the puzzle
    // Failure consequences
    // Discovered clues/ unlocked status
  }

  void show(Components c)
  {
    // TO-DO:
  }
}

#endif