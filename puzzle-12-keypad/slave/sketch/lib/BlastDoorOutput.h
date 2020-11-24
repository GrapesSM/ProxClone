/*
  BlastDoorOutput.h
*/
#ifndef BlastDoorOutput_h
#define BlastDoorOutput_h

#include <Arduino.h>

class BlastDoorOutput
{
  public:
    BlastDoorOutput();
    void init();
    void set(int pin);
    void update();
    STATE getState();
    void setState(STATE state);
    void setOutput(int value);

  private:
    int _pin;
    int _value;
    STATE _state;
};

BlastDoorOutput::BlastDoorOutput() {
  init();
}

void BlastDoorOutput::init() {
  _value = LOW;
}

void BlastDoorOutput::set(int pin) 
{
  _pin = pin;
}

STATE BlastDoorOutput::getState()
{
  return _state;
}

void BlastDoorOutput::setState(STATE state)
{
  _state = state;
}

void BlastDoorOutput::setOutput(int value)
{
  _value = value;
  digitalWrite(_pin, _value);
}

#endif