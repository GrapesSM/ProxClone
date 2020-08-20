/*
  PowerSwitch.h - Library for playing sounds and voices.
*/
#ifndef PowerSwitch_h
#define PowerSwitch_h

#include <Arduino.h>

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
    int _pin1;
    int _pin2;
    STATE _state;
};

PowerSwitch::PowerSwitch() {}

void PowerSwitch::set(int pin1, int pin2) {
  _pin1 = pin1;
  _pin2 = pin2;
}

void PowerSwitch::update() 
{
  if (digitalRead(_pin1) == HIGH) {
    _state = ON;
  } else {
    _state = OFF;
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