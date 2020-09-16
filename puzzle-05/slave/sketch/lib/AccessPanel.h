/*
  AccessPanel.h - Library for _______.
*/
#ifndef AccessPanel_h
#define AccessPanel_h

#include <Arduino.h>
#include "DebounceSwitch.h"

class AccessPanel
{
  public:
    AccessPanel();
    void init();
    void set(int keyPin, int solenoidPin);
    bool keyInserted();
    bool isClosed();
    void open();
    void reset();
    void display();
    void update();
    void setState(STATE state);
    STATE getState();
  private:
    DebounceSwitch _dSwitch;
    int _solenoidPin;
    bool _closed;
    STATE _state;
};

AccessPanel::AccessPanel() 
{
  init();
}

void AccessPanel::init()
{
  _closed = true;
}


void AccessPanel::set(int keyPin, int solenoidPin) 
{
  _dSwitch.set(keyPin);
  _solenoidPin = solenoidPin;
}

bool AccessPanel::keyInserted()
{
  return _dSwitch.getState();
}

void AccessPanel::update()
{
  _dSwitch.readSwitch();  
  switch (_state)
  {
    case DISABLE:
      
      break;

    case SOLVED:

      break;

    case ENABLE:
    default:
      if (keyInserted()) {
        _state = SOLVED;
        if (isClosed()) {
          open();
        }
      } else {
        reset();
      }
      break;
  }
}

bool AccessPanel::isClosed()
{
  return _closed;
}

void AccessPanel::open()
{
    //pulseOut(solenoidPin);
  digitalWrite(_solenoidPin, HIGH);
  delay(200);
  digitalWrite(_solenoidPin, LOW);
  _closed = false;
}

void AccessPanel::reset()
{
  // TO-DO:
  _closed = true;
}

void AccessPanel::display()
{
  // TO-DO:
}

void AccessPanel::setState(STATE state)
{
  _state = state;
}

STATE AccessPanel::getState()
{
  return _state;
}

#endif