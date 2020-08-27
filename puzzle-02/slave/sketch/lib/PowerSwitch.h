/*
  PowerSwitch.h - Library for playing sounds and voices.
*/
#ifndef PowerSwitch_h
#define PowerSwitch_h

#include <Arduino.h>
#include "DebounceSwitch.h"

class PowerSwitch
{
  public:
    PowerSwitch();
    void set(int pin1, int _pin2);
    void display();
    void update();
    void setState(STATE state);
    STATE getState();

  private:
    DebounceSwitch _dSwitch[2];
    STATE _state;
};

PowerSwitch::PowerSwitch() {}

void PowerSwitch::set(int pin1, int pin2) {
  _dSwitch[0].set(pin1);
  _dSwitch[1].set(pin2);
}

void PowerSwitch::update() 
{
  _dSwitch[0].readSwitch();
  _dSwitch[1].readSwitch();
  if (_dSwitch[0].getState() && ! _dSwitch[1].getState()) {
    _state = ON;
  } 

  if (!_dSwitch[0].getState() && _dSwitch[1].getState()) {
    _state = OFF;
  } 

  if (!_dSwitch[0].getState() && ! _dSwitch[1].getState()) {
    _state = RESET;
  } 
}

void PowerSwitch::setState(STATE state)
{
  _state = state;
}

STATE PowerSwitch::getState()
{
  return _state;
}

#endif