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
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin, int pin, bool reversed);
    void setLightOn();
    void setLightOff();
    void display();
    bool isLightOn();
    bool isLightOff();
    bool isSwitchOn();
    bool isSwitchOff();
    void listen();
    void update();
    STATE getState();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int _lightPin;
    int _pin;
    bool _reversed;
    STATE _state;
};

PowerSwitch::PowerSwitch() {}

void PowerSwitch::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin, int pin, bool reversed = false) 
{
  _strip = strip;
  _lightPin = lightPin;
  _pin = pin;
  _state = OFF;
  _reversed = reversed;
}

bool PowerSwitch::isSwitchOn()
{
  return _reversed ? ! digitalRead(_pin) : digitalRead(_pin);
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
  }

  if (isSwitchOn()) {
    _state = ON;
  }
}

STATE PowerSwitch::getState()
{
  return _state;
} 
#endif