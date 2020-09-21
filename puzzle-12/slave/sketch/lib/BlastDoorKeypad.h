#ifndef BlastDoorKeypad_h
#define BlastDoorKeypad_h

#include <Arduino.h>
#include "CodeReader.h"
#include "Speaker.h"

namespace BlastDoorKeypad {
  typedef struct
  {
    CodeReader codeReader;
    Speaker speaker;
    STATE state;
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
  } Components;

  void update(Puzzle &p, Components &c)
  {

    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.state = ENABLE;
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_DISABLE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.state = DISABLE;
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }
    
    if (p.registers[REG_MASTER_COMMAND] == CMD_RESET &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.state = RESET;
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_CODE_READER_STATE] = c.codeReader.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();

    if (c.state == SETUP) {
      c.state = INITIALIZING;

      c.codeReader.init();
      c.speaker.init();

      c.codeReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.state = DISABLE;
    }
  }

  void run(Components &c)
  {
    c.codeReader.update();
    
    if (c.state == DISABLE) {
      c.codeReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
    }
    
    if (c.state == RESET) {
      c.state = SETUP;
    }    
    
    if (c.state == ENABLE) {
      if (c.codeReader.getState() == DISABLE) {
        c.codeReader.setState(ENABLE);
      }

      if (c.codeReader.getState() == KEY_ENTERED) {
        c.speaker.addToPlay(SOUND_ENTERED);
      }

      if (c.codeReader.getState() == TRANSMITTED) {
        
        if (c.codeReader.getInputKey() == keyForCodeReader) {
          c.speaker.addToPlay(SOUND_CORRECT);
          c.codeReader.setState(SOLVED);
        } else {
          c.speaker.addToPlay(SOUND_WRONG);
          c.codeReader.setState(ENABLE);
        }
        
        c.codeReader.clear();

        if (c.codeReader.getState() == SOLVED) {
          c.state = SOLVED;
        }
      } 
    }

    if (c.state == SOLVED) {
      // Serial.println("SOLVED");
    }
  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if (c.showTimer.current - c.showTimer.showpoint > c.showTimer.interval) {
      c.showTimer.showpoint = millis();

      // code here runs every interval, i.e. 200ms 
    }
    c.speaker.play();
  }
} // namespace BlastDoorKeypad

#endif