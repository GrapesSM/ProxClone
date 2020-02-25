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
    STATE state = ON;
  } Components;

  void update(Puzzle &p, Components &c)
  {
    p.registers[REG_POWER_STATE] = ON;
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_CODE_READER_STATE] = c.codeReader.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();

    if (p.registers[REG_MASTER_COMMAND] == CMD_DISABLE_CODE_READER &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      c.codeReader.setState(DISABLE);
      p.registers[REG_MASTER_CONFIRM] = DONE;
    }
    
    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE_CODE_READER &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      c.codeReader.setState(ENABLE);
      p.registers[REG_MASTER_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE_SPEAKER &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      c.speaker.setState(ENABLE);
      p.registers[REG_MASTER_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_DISABLE_SPEAKER &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      c.speaker.setState(DISABLE);
      p.registers[REG_MASTER_CONFIRM] = DONE;
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
      c.state = UNSOLVED;
    }

    if (c.state == UNSOLVED) {
      c.codeReader.update();

      if (c.codeReader.getState() == KEY_ENTERED) {
        c.speaker.speak();
        c.codeReader.setState(ENABLE);
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