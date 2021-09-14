#ifndef BlastDoorKeypad_h
#define BlastDoorKeypad_h

#include <Arduino.h>
#include "CodeReader.h"
#include "Speaker.h"
#include "BlastDoorOutput.h"

namespace BlastDoorKeypad {
  typedef struct
  {
    CodeReader codeReader;
    Speaker speaker;
    BlastDoorOutput blastDoorOutput;
    STATE state;
    struct ShowTimer {
      unsigned long current = 0;
      unsigned long showpoint = 0;
      unsigned long interval = 200;
    } showTimer;
    int codeReaderStateChange[2] = {0, 0};
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

    if (p.registers[REG_MASTER_COMMAND] == CMD_SET_KEYPAD_OUTPUT_1 && 
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      p.registers[REG_SLAVE_CONFIRM] = DONE;
      
      if (p.registers[REG_MASTER_BODY] == 1) {
        c.blastDoorOutput.setOutput(LOW);
      } else {
        c.blastDoorOutput.setOutput(HIGH);
      }
      p.registers[REG_MASTER_BODY] = 0;
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

      c.blastDoorOutput.setOutput(HIGH);
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

      if (c.speaker.getState() == DISABLE) {
        c.speaker.setState(ENABLE);
      }

      if (c.codeReader.getState() == TRANSMITTED) {
        if (c.codeReader.getInputKey() == keyForCodeReader) {
          c.speaker.setCurrent(SOUND_CORRECT);
          c.speaker.setRepeat(false);
          c.speaker.setPlayPartly(false);  
          c.codeReader.setState(SOLVED);
        } else {
          c.speaker.setCurrent(SOUND_ERROR);
          c.speaker.setRepeat(false);
          c.speaker.setPlayPartly(false);  
          c.codeReader.setState(ENABLE);
        }
        
        c.codeReader.clear();
      }

      if (c.codeReader.getState() == SOLVED) {
        c.state = SOLVED;
      }
    }

    if (c.state == SOLVED) {
      // Serial.println("SOLVED");
      c.speaker.setState(DISABLE);
      c.blastDoorOutput.setOutput(LOW);
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

  void sound(Components & c)
  {
    c.codeReaderStateChange[1] = c.codeReader.getState();
    if (c.codeReaderStateChange[0] != c.codeReaderStateChange[1]) {
      c.codeReaderStateChange[0] = c.codeReaderStateChange[1];
      if (c.codeReader.getState() == KEY_ENTERED) {
        c.speaker.setCurrent(SOUND_NUMBER_BUTTONS);
        c.speaker.setRepeat(false);
        c.speaker.setPlayPartly(true);  
      }
    }

    c.speaker.play(100);
  }
} // namespace BlastDoorKeypad

#endif