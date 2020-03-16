/*
  AccessPanel.h - Library for _______.
*/
#ifndef AccessPanel_h
#define AccessPanel_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class AccessPanel
{
  public:
    AccessPanel();
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
    int _keyPin;
    int _solenoidPin;
    bool _closed;
    bool _keyInserted;
    STATE _state;
};

AccessPanel::AccessPanel() {}

void AccessPanel::set(int keyPin, int solenoidPin) 
{
  _keyPin = keyPin;
  _solenoidPin = solenoidPin;
  _closed = true;
  _keyInserted = false;
}

bool AccessPanel::keyInserted()
{
  return digitalRead(_keyPin);
}

void AccessPanel::update()
{
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