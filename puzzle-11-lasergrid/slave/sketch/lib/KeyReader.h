/*
  KeyReader.h - Library for ________.
*/
#ifndef KeyReader_h
#define KeyReader_h

#include <Arduino.h>
#include "DebounceSwitch.h"

class KeyReader
{
  public:
    KeyReader();
    void init();
    void set(int inputPin1, int inputPin2, int inputPin3, int outputPin);
    void update();
    void access();
    void latch();
    void unlatch();
    bool isAllInserted();
    bool isInserted(int number);
    bool getKeyState(int i);
    void setState(STATE state);
    STATE getState();
  private:
    DebounceSwitch _dSwitch[3];
    int _outputPin;
    int _keys[3];
    STATE _state;
    bool _accessed;
};

KeyReader::KeyReader()
{
  init();
}

void KeyReader::init()
{
  _accessed = false;
  _keys[0] = -1;
  _keys[1] = -1;
  _keys[2] = -1;
}

void KeyReader::set(int inputPin1, int inputPin2, int inputPin3, int outputPin)
{
  _dSwitch[0].set(inputPin1);
  _dSwitch[1].set(inputPin2);
  _dSwitch[2].set(inputPin3);
  _outputPin = outputPin;
}

bool KeyReader::isInserted(int number)
{
  return _keys[number - 1];
}

bool KeyReader::isAllInserted()
{
  return _keys[0] && _keys[1] && _keys[2];
}

void KeyReader::update()
{
  _dSwitch[0].readSwitch();
  _dSwitch[1].readSwitch();
  _dSwitch[2].readSwitch();

  if (_state == DISABLE) {
    _keys[0] = LOW;
    _keys[1] = LOW;
    _keys[2] = LOW;  
    return;
  }
  
  _keys[0] = _dSwitch[0].getState();
  _keys[1] = _dSwitch[1].getState();
  _keys[2] = _dSwitch[2].getState();
  
  if (isAllInserted()) {
    if (_accessed == false)
      access();
    _state = SOLVED;
  }
}

bool KeyReader::getKeyState(int i = -1) 
{
  if (i == -1)
    return _keys[0] || _keys[1] || _keys[2];
  
  return _keys[i];
}

void KeyReader::setState(STATE state)
{
  _state = state;
}

STATE KeyReader::getState()
{
  return _state;
}

void KeyReader::access()
{
  _accessed = true;
  latch();
  delay(1000);
  unlatch();
  delay(1000);
}

void KeyReader::latch()
{
  digitalWrite(_outputPin, HIGH);
}

void KeyReader::unlatch()
{
  digitalWrite(_outputPin, LOW);
}

#endif