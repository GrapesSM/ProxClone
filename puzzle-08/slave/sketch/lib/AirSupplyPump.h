/*
  AirSupplyPump.h - Library for _______.
*/
#ifndef AirSupplyPump_h
#define AirSupplyPump_h

#include <Arduino.h>
#include "DebounceSwitch.h"

class AirSupplyPump
{
  public:
    AirSupplyPump();
    void init();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin);
    void update();
    void setState(STATE state);
    STATE getState();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int * _lightPins;
    DebounceSwitch _dSwitch;
    int _switchState;
    int _lastSwitchState;
    STATE _state;
};

AirSupplyPump::AirSupplyPump()
{
  init();
}

void AirSupplyPump::init()
{
}

void AirSupplyPump::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin)
{
  _strip = strip;
  _lightPins = lightPins;
  _dSwitch.set(pin);
}

void AirSupplyPump::update()
{
  _dSwitch.readSwitch();
  if (_state == DISABLE) {
    _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
    _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
    return;
  } 
  
  if (_dSwitch.isSwitch(LOW)) {
    _state = ON;
    _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
    _strip->SetPixelColor(_lightPins[1], RgbColor(255, 255, 255));
  }

  if (_dSwitch.isSwitch(HIGH)) {
    _state = OFF;
    _strip->SetPixelColor(_lightPins[0], RgbColor(255, 255, 255));
    _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
  }
}

void AirSupplyPump::setState(STATE state)
{
  _state = state;
}

STATE AirSupplyPump::getState()
{
  return _state;
}

#endif