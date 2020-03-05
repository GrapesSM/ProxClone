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
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
  } Components;

  void update(Puzzle & p, Components & c)
  {

    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.shipPrepStatus.setState(DISABLE);
      c.lifeSupportStatus.setState(DISABLE);
      c.blastDoorStatus.setState(DISABLE);
      c.laserGrid.setState(DISABLE);
      c.state = INITIALIZED;
    }
  }

  void run(Components & c) 
  {
    if (c.state == INITIALIZED) {
      c.shipPrepStatus.setState(DISABLE);
      c.lifeSupportStatus.setState(DISABLE);
      c.blastDoorStatus.setState(DISABLE);
      c.laserGrid.setState(DISABLE);
    }

    c.shipPrepStatus.update();
    c.lifeSupportStatus.update();
    c.blastDoorStatus.update();
    c.laserGrid.update();

    if (c.state == ENABLE) {
      if (c.shipPrepStatus.getState() == SOLVED) {
        c.shipPrepStatus.setGreenLight(ON);
        c.shipPrepStatus.setRedLight(OFF);
      } else {
        c.shipPrepStatus.setRedLight(ON);
        c.shipPrepStatus.setGreenLight(OFF);
      }

      if (c.lifeSupportStatus.getState() == SOLVED) {
        c.lifeSupportStatus.setRedLight(OFF);
        c.lifeSupportStatus.setGreenLight(ON);
      } else [
        c.lifeSupportStatus.setRedLight(ON);
        c.lifeSupportStatus.setGreenLight(OFF);
      ]

      if (c.blastDoorStatus.getState() == SOLVED) {
        c.blastDoorStatus.setRedLight(OFF);
        c.blastDoorStatus.setGreenLight(ON);
      } else {
        c.blastDoorStatus.setRedLight(ON);
        c.blastDoorStatus.setGreenLight(OFF);
      }

      if (c.laserGrid.getState() == SOLVED) {
        c.laserGrid.setRedLightOff();
        c.laserGrid.setGreenLightOn();
      } else {
        c.laserGrid.setRedLightOn();
        c.laserGrid.setGreenLightOff();
      }
    }

    if (c.state == DISABLE) {
      c.shipPrepStatus.setState(DISABLE);
      c.lifeSupportStatus.setState(DISABLE);
      c.blastDoorStatus.setState(DISABLE);
      c.laserGrid.setState(DISABLE);
    }

    if (c.state == RESET) {

    }

    if (c.state == PAUSE) {

    }
    
  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if (c.showTimer.current - c.showTimer.showpoint > c.showTimer.interval) {
      c.showTimer.showpoint = millis();
      c.shipPrepStatus.display();
      c.lifeSupportStatus.display();
      c.blastDoorStatus.display();
      c.laserGrid.display();
    }
    
  }
}

#endif