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

    if (p.registers[REG_MASTER_COMMAND] == CMD_DISABLE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.state = DISABLE;
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }
    
    if (p.registers[REG_MASTER_COMMAND] == CMD_ENABLE &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.state = ENABLE;
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    if (p.registers[REG_MASTER_COMMAND] == CMD_RESET &&
        p.registers[REG_SLAVE_CONFIRM] != DONE) {
      c.state = RESET;
      p.registers[REG_SLAVE_CONFIRM] = DONE;
    }

    p.registers[REG_SLAVE_MILLIS] = millis();
    p.registers[REG_SLAVE_STATE] = c.state;
    p.registers[REG_SLAVE_DETECTOR_STATE] = c.detector.getState();
    p.registers[REG_SLAVE_SPEAKER_STATE] = c.speaker.getState();

    if (c.state == SETUP) {
      c.state = INITIALIZING;

      c.detector.init();
      c.speaker.init();

      c.detector.setState(DISABLE);
      c.speaker.setState(DISABLE);
      c.state = DISABLE;
    }
  }

  void run(Components &c)
  {
    c.detector.update();
    c.speaker.update();

    if (c.state == DISABLE) {
      c.detector.setState(DISABLE);
      c.speaker.setState(DISABLE);
    }

    if (c.state == RESET) {
      c.state = SETUP;
    }

    if (c.state == ENABLE) {
      if (c.detector.getState() == DISABLE) {
        c.detector.setState(ENABLE);
      }

      if (c.detector.getState() == DETECTED) {
        c.speaker.addToPlay(SOUND_DETECTED);
        c.detector.setState(ENABLE);
      }       
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