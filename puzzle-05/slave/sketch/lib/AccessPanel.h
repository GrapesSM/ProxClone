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
    // void enable();
    // void disable();
    // bool isEnabled();
    STATE _state;
  private:
    int _keyPin;
    int _solenoidPin;
    bool _closed;
    bool _keyInserted;
};

AccessPanel::AccessPanel() {}

void AccessPanel::set(int keyPin, int solenoidPin) 
{
  _keyPin = keyPin;
  _solenoidPin = solenoidPin;
  _state = OFF;
  _closed = true;
  _keyInserted = false;
}

bool AccessPanel::keyInserted()
{
    _keyInserted = digitalRead(_keyPin);
    //Serial.println(_keyInserted);
    return _keyInserted;
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

#endif