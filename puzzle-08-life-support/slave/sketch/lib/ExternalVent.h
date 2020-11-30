/*
  ExternalVent.h - Library for playing sounds and voices.
*/
#ifndef ExternalVent_h
#define ExternalVent_h

#include <Arduino.h>
#include "DebounceSwitch.h"

class ExternalVent
{
  public:
    ExternalVent();
    void init();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin);
    void update();
    void setState(STATE state);
    STATE getState();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int * _lightPins;
    DebounceSwitch _dSwitch;
    STATE _state;
};

ExternalVent::ExternalVent()
{
  init();
}

void ExternalVent::init() 
{
}

void ExternalVent::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin)
{
  _strip = strip;
  _lightPins = lightPins;
  _dSwitch.set(pin);
}

void ExternalVent::update()
{
  _dSwitch.readSwitch();
  if (_state == DISABLE) {
    _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
    _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
    return;
  } 
  
  if (_dSwitch.isSwitch(HIGH)) {
    _state = CLOSED;
    _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
    _strip->SetPixelColor(_lightPins[1], RgbColor(255, 0, 0));
  }

  if (_dSwitch.isSwitch(LOW)) {
    _state = OPEN;
    _strip->SetPixelColor(_lightPins[0], RgbColor(0, 255, 0));
    _strip->SetPixelColor(_lightPins[1], RgbColor(0,0,0));
  }
}

void ExternalVent::setState(STATE state)
{
  _state = state;
}

STATE ExternalVent::getState()
{
  return _state;
}

#endif