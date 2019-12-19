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
    c.shipPrepStatus.listen();
    c.lifeSupportStatus.listen();
    c.blastDoorStatus.listen();
    c.laserGrid.listen();

    if (c.shipPrepStatus.isSolved()) {
      c.shipPrepStatus.setGreenLightOn();
      c.shipPrepStatus.setRedLightOff();
    } else {
      c.shipPrepStatus.setRedLightOn();
      c.shipPrepStatus.setGreenLightOff();
    }

    if (c.lifeSupportStatus.isSolved()) {
      c.lifeSupportStatus.setRedLightOff();
      c.lifeSupportStatus.setGreenLightOn();
    } else [
      c.lifeSupportStatus.setRedLightOn();
      c.lifeSupportStatus.setGreenLightOff();
    ]

    if (c.blastDoorStatus.isSolved()) {
      c.blastDoorStatus.setRedLightOff();
      c.blastDoorStatus.setGreenLightOn();
    } else {
      c.blastDoorStatus.setRedLightOn();
      c.blastDoorStatus.setGreenLightOff();
    }

    if (c.laserGrid.isSolved()) {
      c.laserGrid.setRedLightOff();
      c.laserGrid.setGreenLightOn();
    } else {
      c.laserGrid.setRedLightOn();
      c.laserGrid.setGreenLightOff();
    }
  }

  void show(Components c)
  {
    c.shipPrepStatus.display();
    c.lifeSupportStatus.display();
    c.blastDoorStatus.display();
    c.laserGrid.display();
  }
}

#endif