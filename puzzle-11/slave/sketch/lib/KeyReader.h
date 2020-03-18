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
    void setState(STATE state);
    STATE getState();
  private:
    int _inputPin1;
    int _inputPin2;
    int _inputPin3;
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
  _inputPin1 = inputPin1;
  _inputPin2 = inputPin2;
  _inputPin3 = inputPin3;
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
  if (_state == DISABLE) {
    _keys[0] = LOW;
    _keys[1] = LOW;
    _keys[2] = LOW;  
    return;
  }
  
  _keys[0] = digitalRead(_inputPin1);
  _keys[1] = digitalRead(_inputPin2);
  _keys[2] = digitalRead(_inputPin3);
  
  if (isAllInserted()) {
    if (_accessed == false)
      access();
    _state = SOLVED;
  }
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
  // Serial2.print("page 1");
  // Serial2.write(0xff);
  // Serial2.write(0xff);
  // Serial2.write(0xff);
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