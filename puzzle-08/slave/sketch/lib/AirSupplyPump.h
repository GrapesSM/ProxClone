/*
  AirSupplyPump.h - Library for playing sounds and voices.
*/
#ifndef AirSupplyPump_h
#define AirSupplyPump_h

#include <Arduino.h>

class AirSupplyPump
{
  public:
    AirSupplyPump();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin);
    void update();
    bool isSwitch(int position);
    void setState(STATE state);
    STATE getState();
  private:
    STATE _state;
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int * _lightPins;
    int _pin;
};

AirSupplyPump::AirSupplyPump()
{

}

void AirSupplyPump::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin)
{
  _strip = strip;
  _lightPins = lightPins;
  _pin = pin;
}

void AirSupplyPump::update()
{
  if (_state == DISABLE) {
    _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
    _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
    return;
  } 
  
  if (isSwitch(LOW)) {
    _state = ON;
    _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
    _strip->SetPixelColor(_lightPins[1], RgbColor(255, 255, 255));
  }

  if (isSwitch(HIGH)) {
    _state = OFF;
    _strip->SetPixelColor(_lightPins[0], RgbColor(255, 255, 255));
    _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
  }
}

bool AirSupplyPump::isSwitch(int position) 
{
  return digitalRead(_pin) == position;
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