/*
  KeyReader.h - Library for ________.
*/
#ifndef KeyReader_h
#define KeyReader_h

#include <Arduino.h>

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
    bool isSolved();
    void setSolved(bool value);   
    void disable();
    void enable();
    bool isDisabled();
    STATE getState();
  private:
    int _inputPin1;
    int _inputPin2;
    int _inputPin3;
    int _outputPin;
    int _keys[3];
    bool _solved;
    bool _disabled;
    STATE _state;
};

KeyReader::KeyReader()
{
  init();
}

void KeyReader::init()
{
  _solved = false;
  _disabled = true;
  _keys[0] = -1;
  _keys[1] = -1;
  _keys[2] = -1;
}

void KeyReader::set(int inputPin1, int inputPin2, int inputPin3, int outputPin)
{
  _inputPin1 = inputPin1;
  _inputPin2 = inputPin2;
  _inputPin3 = inputPin3;
  _outputPin = outputPin;
}

bool KeyReader::isInserted(int number)
{
  return _keys[number - 1];
}

void KeyReader::setSolved(bool value = true)
{
  _solved = value;
}

bool KeyReader::isSolved()
{
  return _solved;
}

void KeyReader::disable()
{
  _state = OFF;
  _disabled = true;
}

void KeyReader::enable()
{
  _state = ON;
  _disabled = false;
}

bool KeyReader::isDisabled()
{
  return _disabled;
}

bool KeyReader::isAllInserted()
{
  return _keys[0] && _keys[1] && _keys[2];
}

void KeyReader::update()
{
  if (_disabled) {
    return;
  } 

  _state = READING;
  _keys[0] = digitalRead(_inputPin1);
  _keys[1] = digitalRead(_inputPin2);
  _keys[2] = digitalRead(_inputPin3);
  
  if (isAllInserted()) {
    _solved = true;
    _state = SOLVED;
  } else {
    _solved = false;
  }
}

STATE KeyReader::getState()
{
  return _state;
}

void KeyReader::access()
{
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