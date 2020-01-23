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
    void set();
    void disable();
    void enable();
    void update();
    void display();
    bool isDisabled();
    bool isSolved();
  private:
    bool _disabled;
    bool _solved;
};

KeyReader::KeyReader() {}

void KeyReader::set()
{
  _solved = false;
  _disabled = true;
}

void KeyReader::disable()
{
  _disabled = true;
  // TO-DO:
}

void KeyReader::enable()
{
  _disabled = false;
  // TO-DO:
}

bool KeyReader::isDisabled()
{
  return _disabled;
}

void KeyReader::update()
{
  // TO-DO:
}

void KeyReader::display()
{
  _strip->Show();
}

bool KeyReader::isSolved()
{
  return _solved;
}

#endif