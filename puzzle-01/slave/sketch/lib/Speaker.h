/*
  Speaker.h - Library for playing sounds and voices.
*/
#ifndef Speaker_h
#define Speaker_h

#include <Arduino.h>

class Speaker
{
  public:
    Speaker();
    void init();
    void set(int pin, int enablePin);
    void setState(STATE state);
    STATE getState();
    void update();
    void setNumber(int number);
    int getNumber();
    void play();
  private:
    int _pin;
    int _enablePin;
    int _number;
    STATE _state;
};

Speaker::Speaker() 
{
  init();
}

void Speaker::init()
{
  _number = -1;
}

void Speaker::set(
  int pin, 
  int enablePin)
{
  _pin = pin;
  _enablePin = enablePin;
}

void Speaker::setState(STATE state)
{
  _state = state;
}

STATE Speaker::getState()
{
  return _state;
}

void Speaker::setNumber(int number) 
{
  _number = number;
}

int Speaker::getNumber()
{
  return _number;
}

void Speaker::update()
{
  
}

void Speaker::play() 
{
  
}

#endif