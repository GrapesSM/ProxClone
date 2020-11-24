/*
  DebounceSwitch.h
*/
#ifndef DebounceSwitch_h
#define DebounceSwitch_h

#include <Arduino.h>

class DebounceSwitch
{
  public:
    DebounceSwitch();
    void set(int pin);
    void update();
    void readSwitch();
    bool getState();
    bool isSwitch(int position); // position: HIGH or LOW

  private:
    int _pin;
    int _reading;
    int _switchState;
    int _lastSwitchState;
    unsigned long _lastDebounceTime = 0;
    unsigned long _debounceDelay = 50;
};

DebounceSwitch::DebounceSwitch() {}

void DebounceSwitch::set(int pin) 
{
  _pin = pin;
}

void DebounceSwitch::readSwitch()
{
  _reading = digitalRead(_pin);
  if (_reading != _lastSwitchState) {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    // if (_reading != _switchState) {
      _switchState = _reading;
    // }
  }

  _lastSwitchState = _reading;
}

bool DebounceSwitch::getState()
{
  return _switchState;
}

bool DebounceSwitch::isSwitch(int position)
{
  return _switchState == position;
}

#endif