#ifndef LaserBar_h
#define LaserBar_h

#include <Arduino.h>
#include "Detector.h"
#include "Speaker.h"

namespace LaserBar {
  typedef struct
  {
    Detector detector;
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
    p.registers[REG_POWER_STATE] = ON;
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_DETECTOR_STATE] = c.detector.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();

    if (p.registers[REG_MASTER_COMMAND] == CMD_DISABLE_DETECTOR &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      c.detector.setState(DISABLE);
      p.registers[REG_MASTER_CONFIRM] = DONE;
    }
    
    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE_DETECTOR &&
        p.registers[REG_MASTER_CONFIRM] != DONE) {
      c.detector.setState(ENABLE);
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
      c.detector.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.state = INITIALIZED;
    }
  }

  void run(Components &c)
  {
    if (c.state == INITIALIZED) {
      c.detector.setState(ENABLE);
      c.speaker.setState(ENABLE);
      c.state = UNSOLVED;
    }

    if (c.state == UNSOLVED) {
      c.detector.update();

      if (c.detector.getState() == DETECTED) {
        c.speaker.speak();
        c.detector.setState(ENABLE);
        delay(10);
        c.speaker.speak(0);
      }       
    }

    if (c.state == SOLVED) {
      Serial.println("SOLVED");
    }
  }

  void show(Components & c)
  {
    c.showTimer.current = millis();
    if (c.showTimer.current - c.showTimer.showpoint > c.showTimer.interval) {
      c.showTimer.showpoint = millis();

      c.detector.display();
    }
    c.speaker.play();
  }
} // namespace LaserBar

#endif