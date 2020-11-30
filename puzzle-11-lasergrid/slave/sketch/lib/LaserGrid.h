/*
  LaserGrid.h - Library for _________.
*/
#ifndef LaserGrid_h
#define LaserGrid_h

#include <Arduino.h>
#include "WaveAdjuster.h"
#include "KeyReader.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace LaserGrid {
  typedef struct {
    WaveAdjuster waveAdjuster;
    KeyReader keyReader;
    PowerSwitch powerSwitch;
    Speaker speaker;
    STATE state;
    boolean in;
    boolean flag;
    boolean keyInserted[3];
    int val = 0;
    struct Timer {
      unsigned long start = 0;
      unsigned long end = 0;
      unsigned long current = 0;
    } timer;
    struct WaveTimer {
      unsigned long current = 0;
      unsigned long point = 0;
      unsigned long interval = 8;
    } waveTimer;
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
    int powerSwitchStateChange[2] = {0, 0}; // old, new
    int keyReaderInputChange[3][2] = {
      {0, 0},
      {0, 0},
      {0, 0}
    }; // old, new
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

    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_POWER_SWITCH_STATE] = c.powerSwitch.getState();
    p.registers[REG_SLAVE_KEY_READER_STATE] = c.keyReader.getState();
    p.registers[REG_SLAVE_WAVE_ADJUSTER_STATE] = c.waveAdjuster.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();

    if (c.state == SETUP) {
      c.state = INITIALIZING;

      c.powerSwitch.init();
      c.keyReader.init();
      c.waveAdjuster.init();
      c.speaker.init();

      c.powerSwitch.setState(DISABLE);
      c.keyReader.setState(DISABLE);
      c.waveAdjuster.setState(DISABLE); 
      c.speaker.setState(DISABLE);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      c.state = DISABLE;
    }
  }

  void run(Components & c) 
  {  
    c.powerSwitch.update();
    c.keyReader.update();
    c.waveAdjuster.update();
    c.speaker.update();

    if (c.powerSwitch.getState() == DISABLE) {
      c.powerSwitch.setState(ENABLE);
    }

    if (c.state == DISABLE) {
      c.powerSwitch.setLightOff();
      c.keyReader.setState(DISABLE);
      c.waveAdjuster.setState(DISABLE); 
      c.speaker.setState(DISABLE);
    }

    if (c.state == RESET) {
      c.state = SETUP;
    }

    if (c.state == ENABLE) {
      
      if (c.powerSwitch.getState() == OFF) { 
        c.keyReader.setState(DISABLE);
        c.waveAdjuster.setState(DISABLE);
      }

      if (c.powerSwitch.getState() == ON) {
        if (c.speaker.getState() == DISABLE) {
          c.speaker.setState(ENABLE);
        }

        if (c.keyReader.getState() == DISABLE) {
          c.keyReader.setState(ENABLE);
        }

        if (c.keyReader.getKeyState(0) && c.keyInserted[0] == false) {
          c.keyInserted[0] = true;
        }

        if (c.keyReader.getKeyState(1) && c.keyInserted[1] == false) {
          c.keyInserted[1] = true;
        }

        if (c.keyReader.getKeyState(2) && c.keyInserted[2] == false) {
          c.keyInserted[2] = true;
        }

        if (! c.keyReader.getKeyState(0)) c.keyInserted[0] = false;
        if (! c.keyReader.getKeyState(1)) c.keyInserted[1] = false;
        if (! c.keyReader.getKeyState(2)) c.keyInserted[2] = false;
      }

      if (c.keyReader.getState() == SOLVED && c.waveAdjuster.getState() == DISABLE) {
        c.waveAdjuster.setState(ENABLE);
      }
 
      if (
        c.keyReader.getState() == SOLVED && 
        c.waveAdjuster.getState() == SOLVED
      ) {
        c.state = SOLVED;
      }
    }
    
    if (c.state == SOLVED) {
      if (c.powerSwitch.getState() == OFF) { 
        c.keyReader.setState(DISABLE);
        c.waveAdjuster.setState(DISABLE);
      }

      if (c.powerSwitch.getState() == ON) {
        
      }
    }

  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if ((c.showTimer.current - c.showTimer.showpoint) > c.showTimer.interval) {
      c.showTimer.showpoint = millis();

    }
    c.powerSwitch.display();

    c.waveTimer.current = millis();
    if ((c.waveTimer.current - c.waveTimer.point) > c.waveTimer.interval) {
      c.waveTimer.point = millis();
      c.waveAdjuster.display();
    }
  }

  void sound(Components & c)
  {
    c.powerSwitchStateChange[1] = c.powerSwitch.getState();
    if (c.powerSwitchStateChange[0] != c.powerSwitchStateChange[1]) {
      c.powerSwitchStateChange[0] = c.powerSwitchStateChange[1];

      if (c.powerSwitch.getState() == ON) {
        c.speaker.setCurrent(SOUND_STATION_UP);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }

      if (c.powerSwitch.getState() == OFF) {
        c.speaker.setCurrent(SOUND_STATION_DOWN);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }
    }

    c.keyReaderInputChange[0][1] = c.keyReader.getKeyState(0);
    if (c.keyReaderInputChange[0][0] != c.keyReaderInputChange[0][1]) {
      c.keyReaderInputChange[0][0] = c.keyReaderInputChange[0][1];
      if (c.keyReader.getKeyState(0)) {
        c.speaker.setCurrent(SOUND_KEY_INSERT);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      }
    }

    c.keyReaderInputChange[1][1] = c.keyReader.getKeyState(1);
    if (c.keyReaderInputChange[1][0] != c.keyReaderInputChange[1][1]) {
      c.keyReaderInputChange[1][0] = c.keyReaderInputChange[1][1];
      if (c.keyReader.getKeyState(1)) {
        c.speaker.setCurrent(SOUND_KEY_INSERT);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      } 
    }

    c.keyReaderInputChange[2][1] = c.keyReader.getKeyState(2);
    if (c.keyReaderInputChange[2][0] != c.keyReaderInputChange[2][1]) {
      c.keyReaderInputChange[2][0] = c.keyReaderInputChange[2][1];
      if (c.keyReader.getKeyState(2)) {
        c.speaker.setCurrent(SOUND_KEY_INSERT);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(false);
      } 
    }

    c.speaker.play();
  }
}

#endif