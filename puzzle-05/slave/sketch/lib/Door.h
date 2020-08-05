/*
  Door.h - Library for _______.
*/
#ifndef Door_h
#define Door_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class Door
{
  public:
    Door();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPin, int pin);
    void setClosed(bool);
    bool isClosed();
    void open();
    void reset();
    void display();
    void setState(STATE);
    STATE getState();
    STATE _state;
    void update();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int _pin;
    int _lightPin;
    bool _closed;
};

Door::Door() {}

void Door::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPin, int pin) 
{
  _strip = strip;
  _lightPin = lightPin;
  _pin = pin;
  _state = OFF;
  _closed = true;
}

void Door::setClosed(bool value)
{
  _closed = value;
}

bool Door::isClosed()
{
  return _closed;
}

void Door::open()
{
  _closed = false;
  digitalWrite(_pin, HIGH);
  delay(200);
  digitalWrite(_pin, LOW);
  _strip->SetPixelColor(_lightPin, RgbColor(127, 127, 127));
}

void Door::update()
{
  if(_state == DISABLE) 
  {
    _strip->SetPixelColor(_lightPin, RgbColor(0, 0, 0));
    return;
  }
  if(_state == ENABLE)
  {
    if (isClosed()) {
      open();
    } else {
      // reset();
    }
  }
}

void Door::reset()
{
  _strip->SetPixelColor(_lightPin, RgbColor(0, 0, 0));
  _closed = true;
}

void Door::display()
{
  // TO-DO:
}

void Door::setState(STATE state){
  _state = state;
}

STATE Door::getState()
{
  return _state;
}

#endif