/*
  ExternalVent.h - Library for playing sounds and voices.
*/
#ifndef ExternalVent_h
#define ExternalVent_h

#include <Arduino.h>

class ExternalVent
{
  public:
    ExternalVent();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin);
    void update();
    void readSwitch();
    bool isSwitch(int position);
    void setState(STATE state);
    STATE getState();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int * _lightPins;
    int _pin;
    int _reading;
    int _switchState;
    int _lastSwitchState;
    unsigned long _lastDebounceTime = 0;
    unsigned long _debounceDelay = 50;
    STATE _state;
};

ExternalVent::ExternalVent(){}

void ExternalVent::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin)
{
  _strip = strip;
  _lightPins = lightPins;
  _pin = pin;
}

void ExternalVent::readSwitch()
{
  _reading = digitalRead(_pin);
  if (_reading != _lastSwitchState) {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    // if (_reading != _switchState) {
      _switchState = _reading;
    // }
  }

  _lastSwitchState = _reading;
}

void ExternalVent::update()
{
  readSwitch();
  if (_state == DISABLE) {
    _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
    _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
    return;
  } 
  
  if (isSwitch(HIGH)) {
    _state = CLOSED;
    _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
    _strip->SetPixelColor(_lightPins[1], RgbColor(255, 255, 255));
  }

  if (isSwitch(LOW)) {
    _state = OPEN;
    _strip->SetPixelColor(_lightPins[0], RgbColor(255, 255, 255));
    _strip->SetPixelColor(_lightPins[1], RgbColor(0,0,0));
  }
}

bool ExternalVent::isSwitch(int position)
{
  return digitalRead(_pin) == position;
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