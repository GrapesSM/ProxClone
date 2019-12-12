/*
  StatusBoard.h - Library for controlling energy and syncro reader.
*/
#ifndef StatusBoard_h
#define StatusBoard_h

#include <Arduino.h>
#include "ShipPrepStatus.h"
#include "LifeSupportStatus.h"
#include "BlastDoorStatus.h"
#include "LaserGridStatus.h"
#include "Speaker.h"

namespace StatusBoard {
  typedef struct {
    ShipPrepStatus shipPrepStatus;
    LifeSupportStatus lifeSupportStatus;
    BlastDoorStatus blastDoorStatus;
    LaserGrid laserGrid;
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