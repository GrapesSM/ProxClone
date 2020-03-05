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
    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_CODE_READER_STATE] = c.codeReader.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();

    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.codeReader.setState(ENABLE);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_DISABLE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.codeReader.setState(DISABLE);
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    if (c.state == SETUP) {
      c.state = INITIALIZING;
      c.codeReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.state = INITIALIZED;
    }
  }

  void run(Components &c)
  {
    if (c.state == INITIALIZED) {
      
    }

    c.codeReader.update();
    
    if (c.state == ENABLE) {
      if (c.codeReader.getState() == DISABLE) {
        c.codeReader.setState(ENABLE);
      }

      if (c.codeReader.getState() == KEY_ENTERED) {
        c.speaker.speak();
        delay(10);
        c.speaker.speak(0);
      }

      if (c.codeReader.getState() == TRANSMITTED) {
        if (c.codeReader.getInputKey() == keyForCodeReader) {
          c.codeReader.setState(SOLVED);
        } else {
          c.codeReader.setState(ENABLE);
        }
        
        if (c.codeReader.getState() == SOLVED) {
          c.state = SOLVED;
        }

        c.codeReader.clear();
      } 
    }

    if (c.state == SOLVED) {
      Serial.println("SOLVED");
    }

    if (c.state == DISABLE) {
      c.codeReader.setState(DISABLE);
      c.speaker.setState(DISABLE);
    }

    if (c.state == RESET) {

    }    

    if (c.state == PAUSE) {

    }
  }

  void show(Components & c)
  {
    // c.showTimer.current = millis();
    // if (c.showTimer.current - c.showTimer.showpoint > c.showTimer.interval) {
    //   c.showTimer.showpoint = millis();

    // }
    c.speaker.play();
  }
} // namespace BlastDoorKeypad

#endif