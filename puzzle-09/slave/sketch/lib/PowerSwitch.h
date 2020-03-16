
/*
  PowerSwitch.h - Library for playing sounds and voices.
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
    void setState(STATE state);
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
  switch (_state)
  {
    case DISABLE:
      setLightOff();
      break;
    
    default:
      if (isSwitchOff()) {
        _state = OFF;
        setLightOff();
      }

      if (isSwitchOn()) {
        _state = ON;
        setLightOn();
      }
      break;
  }
}

void PowerSwitch::setState(STATE state)
{
  _state = state;
}

STATE PowerSwitch::getState()
{
  return _state;
}

#endif