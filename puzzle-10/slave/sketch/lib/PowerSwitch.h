/*
  PowerSwitch.h - Library for ________.
*/
#ifndef PowerSwitch_h
#define PowerSwitch_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class PowerSwitch
{
  public:
    PowerSwitch();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin, int pin);
    void setLightOn();
    void setLightOff();
    void display();
    void update();
    STATE getState();
    bool isSwitchOn();
    bool isSwitchOff();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int _lightPin;
    int _pin;
    STATE _state;
};

PowerSwitch::PowerSwitch() {}

void PowerSwitch::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin, int pin) 
{
  _strip = strip;
  _lightPin = lightPin;
  _pin = pin;
}

bool PowerSwitch::isSwitchOn()
{
  return digitalRead(_pin);
}

bool PowerSwitch::isSwitchOff()
{
  return ! isSwitchOn();
}

void PowerSwitch::setLightOn()
{
  _strip->SetPixelColor(_lightPin, RgbColor(127, 127, 127));
}

void PowerSwitch::setLightOff()
{
  _strip->SetPixelColor(_lightPin, RgbColor(0, 0, 0));
}

void PowerSwitch::display()
{
  _strip->Show();
}

void PowerSwitch::update() 
{
  if (isSwitchOff()) {
    _state = OFF;
    setLightOff();
  }

  if (isSwitchOn()) {
    _state = ON;
    setLightOn();
  }
}

STATE PowerSwitch::getState()
{
  return _state;
}

#endif