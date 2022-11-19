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
  void init();
  void set(int pin1, int _pin2);
  void display();
  void update();
  void setState(STATE state);
  STATE getState();

private:
  DebounceSwitch _dSwitch[2];
  STATE _state;
  bool _turned = false;
  bool _sw1 = false, _prevSw1 = false;
  bool _sw2 = false, _prevSw2 = false;
  struct Timer
  {
    unsigned long current = 0;
    unsigned long point = 0;
    unsigned long interval = 2000;
  } _timer;
};

PowerSwitch::PowerSwitch()
{
  init();
}

void PowerSwitch::init()
{
}

void PowerSwitch::set(int pin1, int pin2)
{
  _dSwitch[0].set(pin1);
  _dSwitch[1].set(pin2);
}

void PowerSwitch::update()
{
  _dSwitch[0].readSwitch();
  _dSwitch[1].readSwitch();

  _prevSw1 = _sw1;
  _sw1 = _dSwitch[0].getState();
  _prevSw2 = _sw2;
  _sw2 = _dSwitch[1].getState();

  if (_prevSw1 != _sw1 || _prevSw2 != _sw2) {
    _turned = true;
    _timer.point = millis();
    _timer.current = _timer.point;
  }

  if (_turned)
  {
    _timer.current = millis();
    if (_timer.current - _timer.point > _timer.interval)
    {
      _turned == false;

      if (sw1 && !sw2)
      {
        _state = ON;
      }
      else if (!sw1 && sw2)
      {
        _state = OFF;
      }
      else if (!sw1 && !sw2)
      {
        _state = RESET;
      }
    }
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