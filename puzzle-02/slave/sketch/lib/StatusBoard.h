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
#include "Countdown.h"


namespace StatusBoard {
  typedef struct {
    ShipPrepStatus shipPrepStatus;
    LifeSupportStatus lifeSupportStatus;
    BlastDoorStatus blastDoorStatus;
    LaserGridStatus laserGrid;
    Speaker speaker;
    Countdown countdown;
    STATE state;
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
    CountDown countdown;
  } Components;

  void update(Puzzle & p, Components & c)
  {
    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE && 
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = ENABLE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_DISABLE && 
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = DISABLE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_RESET &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = RESET;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_PAUSE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = PAUSE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = SOLVED;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_LASER_GRID_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;   
      c.laserGrid.setState(SOLVED);
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_DOCKED_SHIP_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;   
      c.shipPrepStatus.setState(SOLVED);
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_KEYPAD_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;   
      c.blastDoorStatus.setState(SOLVED);
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_LIFE_SUPPORT_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;   
      c.lifeSupportStatus.setState(SOLVED);
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_COUNT_DOWN_MASTER_TIME &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;   
      c.countdown.setMasterTime([]); 
    }

    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_SHIP_PREP_STATUS_STATE] = c.shipPrepStatus.getState();
    p.registers[REG_SLAVE_LIFE_SUPPORT_STATUS_STATE] = c.lifeSupportStatus.getState();
    p.registers[REG_SLAVE_BLAST_DOOR_STATUS_STATE] = c.blastDoorStatus.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();
    //p.registers[REG_SLAVE_GAME_POWER_SWITCH_STATE] = c.speaker.getState();
    p.registers[REG_SLAVE_COUNTDOWN] = c.countdown.getTime();
    p.registers[REG_SLAVE_COUNTDOWN_STATE] = c.countdown.getState();


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
      c.state = SETUP;
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
      c.countdown.display();
    }
    
  }
}

#endif