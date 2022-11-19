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
#include "PowerSwitch.h"

namespace StatusBoard
{
  typedef struct
  {
    ShipPrepStatus shipPrepStatus;
    LifeSupportStatus lifeSupportStatus;
    BlastDoorStatus blastDoorStatus;
    LaserGridStatus laserGridStatus;
    Speaker speaker;
    Countdown countdown;
    PowerSwitch powerSwitch;
    STATE state;
    struct ShowTimer
    {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
    int countdownValueChange[2] = {0, 0};
  } Components;

  void update(Puzzle &p, Components &c)
  {
    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE)
    {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = ENABLE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_DISABLE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE)
    {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = DISABLE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_RESET &&
        p.registers[REG_SLAVE_CONFIRM] != DONE)
    {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = RESET;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE)
    {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = SOLVED;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_LASER_GRID_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE)
    {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.laserGridStatus.setState(SOLVED);
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_DOCKED_SHIP_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE)
    {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.shipPrepStatus.setState(SOLVED);
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_KEYPAD_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE)
    {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.blastDoorStatus.setState(SOLVED);
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_LIFE_SUPPORT_SOLVED &&
        p.registers[REG_SLAVE_CONFIRM] != DONE)
    {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.lifeSupportStatus.setState(SOLVED);
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_RESET_COUNTDOWN_TIME &&
        p.registers[REG_SLAVE_CONFIRM] != DONE)
    {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      if (p.registers[REG_MASTER_BODY] > 0)
      {
        c.countdown.setmaxTimeCount(p.registers[REG_MASTER_BODY]);
        p.registers[REG_MASTER_BODY] = 0;
      }
      c.countdown.resetTime();
    }

    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_GAME_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_SHIP_PREP_STATUS_STATE] = c.shipPrepStatus.getState();
    p.registers[REG_SLAVE_LIFE_SUPPORT_STATUS_STATE] = c.lifeSupportStatus.getState();
    p.registers[REG_SLAVE_LASER_GRID_STATUS_STATE] = c.laserGridStatus.getState();
    p.registers[REG_SLAVE_BLAST_DOOR_STATUS_STATE] = c.blastDoorStatus.getState();
    p.registers[REG_SLAVE_COUNTDOWN_STATE] = c.countdown.getState();
    p.registers[REG_SLAVE_COUNTDOWN_VALUE] = c.countdown.getTime();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();

    if (c.state == SETUP)
    {
      c.state = INITIALIZING;

      c.shipPrepStatus.init();
      c.lifeSupportStatus.init();
      c.blastDoorStatus.init();
      c.laserGridStatus.init();
      c.countdown.init();

      c.shipPrepStatus.setState(DISABLE);
      c.lifeSupportStatus.setState(DISABLE);
      c.blastDoorStatus.setState(DISABLE);
      c.laserGridStatus.setState(DISABLE);
      c.countdown.setState(DISABLE);
      c.state = ENABLE;
    }
  }

  void run(Components &c)
  {
    c.powerSwitch.update();
    c.shipPrepStatus.update();
    c.lifeSupportStatus.update();
    c.blastDoorStatus.update();
    c.laserGridStatus.update();
    c.countdown.update();

    if (c.state == DISABLE)
    {
      c.shipPrepStatus.setState(DISABLE);
      c.lifeSupportStatus.setState(DISABLE);
      c.blastDoorStatus.setState(DISABLE);
      c.laserGridStatus.setState(DISABLE);
      c.countdown.setState(DISABLE);
    }

    if (c.state == RESET)
    {
      c.state = SETUP;
    }

    if (c.state == ENABLE)
    {

      if (c.shipPrepStatus.getState() == DISABLE)
      {
        c.shipPrepStatus.setState(ENABLE);
      }

      if (c.lifeSupportStatus.getState() == DISABLE)
      {
        c.lifeSupportStatus.setState(ENABLE);
      }

      if (c.blastDoorStatus.getState() == DISABLE)
      {
        c.blastDoorStatus.setState(ENABLE);
      }

      if (c.laserGridStatus.getState() == DISABLE)
      {
        c.laserGridStatus.setState(ENABLE);
      }

      if (c.countdown.getState() == DISABLE)
      {
        c.countdown.setState(ENABLE);
      }

      if (c.shipPrepStatus.getState() == SOLVED)
      {
        c.shipPrepStatus.setGreenLight(ON);
        c.shipPrepStatus.setRedLight(OFF);
      }
      else if (c.shipPrepStatus.getState() == ENABLE)
      {
        c.shipPrepStatus.setRedLight(ON);
        c.shipPrepStatus.setGreenLight(OFF);
      }

      if (c.lifeSupportStatus.getState() == SOLVED)
      {
        c.lifeSupportStatus.setRedLight(OFF);
        c.lifeSupportStatus.setGreenLight(ON);
      }
      else if (c.lifeSupportStatus.getState() == ENABLE)
      {
        c.lifeSupportStatus.setRedLight(ON);
        c.lifeSupportStatus.setGreenLight(OFF);
      }

      if (c.blastDoorStatus.getState() == SOLVED)
      {
        c.blastDoorStatus.setRedLight(OFF);
        c.blastDoorStatus.setGreenLight(ON);
      }
      else if (c.blastDoorStatus.getState() == ENABLE)
      {
        c.blastDoorStatus.setRedLight(ON);
        c.blastDoorStatus.setGreenLight(OFF);
      }

      if (c.laserGridStatus.getState() == SOLVED)
      {
        c.laserGridStatus.setRedLight(OFF);
        c.laserGridStatus.setGreenLight(ON);
      }
      else if (c.laserGridStatus.getState() == ENABLE)
      {
        c.laserGridStatus.setRedLight(ON);
        c.laserGridStatus.setGreenLight(OFF);
      }

      if (c.powerSwitch.getState() == ON) {
        // Serial.println("ON");
        c.countdown.setState(ENABLE);
      } 
      else if (c.powerSwitch.getState() == OFF) {
        // Serial.println("OFF");
        c.countdown.setState(PAUSE);
      } else if (c.powerSwitch.getState() == RESET) {
        // Serial.println("RESET");
        c.countdown.setState(RESET);
      }

      if (c.shipPrepStatus.getState() == SOLVED &&
          c.lifeSupportStatus.getState() == SOLVED &&
          c.blastDoorStatus.getState() == SOLVED &&
          c.laserGridStatus.getState() == SOLVED)
      {
        c.countdown.setState(PAUSE);
      }
    }
  }

  void show(Components &c)
  {
    c.showTimer.current = millis();
    if (c.showTimer.current - c.showTimer.showpoint > c.showTimer.interval)
    {
      c.showTimer.showpoint = millis();
      c.shipPrepStatus.display();
      c.lifeSupportStatus.display();
      c.blastDoorStatus.display();
      c.laserGridStatus.display();
      c.countdown.display();
    }
  }

  void sound(Components &c)
  {
    c.countdownValueChange[1] = c.countdown.getValue();
    if (c.countdownValueChange[0] != c.countdownValueChange[1])
    {
      c.countdownValueChange[0] = c.countdownValueChange[1];

      if (c.countdown.getValue() == 5959)
      {
        c.speaker.setCurrent(SOUND_START);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }

      if (c.countdown.getValue() == 5000)
      {
        c.speaker.setCurrent(SOUND_50MIN);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }

      if (c.countdown.getValue() == 4000)
      {
        c.speaker.setCurrent(SOUND_40MIN);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }

      if (c.countdown.getValue() == 3000)
      {
        c.speaker.setCurrent(SOUND_30MIN);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }

      if (c.countdown.getValue() == 2000)
      {
        c.speaker.setCurrent(SOUND_20MIN);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }

      if (c.countdown.getValue() == 1000)
      {
        c.speaker.setCurrent(SOUND_10MIN);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }

      if (c.countdown.getValue() == 102)
      {
        c.speaker.setCurrent(SOUND_FINAL);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }
    }

    c.speaker.play(150);
  }
}

#endif