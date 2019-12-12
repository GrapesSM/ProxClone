/*
  PowerIndicator.h - Library for playing sounds and voices.
*/
#ifndef PowerIndicator_h
#define PowerIndicator_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class PowerIndicator
{
  public:
    PowerIndicator();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin, int pin);
    void setLightOn();
    void setLightOff();
    void display();
    bool isLightOn();
    bool isLightOff();
    bool isSwitchOn();
    bool isSwitchOff();
    void listen();
    STATE _state;
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int _lightPin;
    int _pin;
};

PowerIndicator::PowerIndicator() {}

void PowerIndicator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin, int pin) 
{
  _strip = strip;
  _lightPin = lightPin;
  _pin = pin;
  _state = OFF;
}

bool PowerIndicator::isSwitchOn()
{
  return digitalRead(_pin);
}

bool PowerIndicator::isSwitchOff()
{
  return ! isSwitchOn();
}

void PowerIndicator::setLightOn()
{
  _strip->SetPixelColor(_lightPin, RgbColor(127, 127, 127));
}

void PowerIndicator::setLightOff()
{
  _strip->SetPixelColor(_lightPin, RgbColor(0, 0, 0));
}

void PowerIndicator::display()
{
  _strip->Show();
}

#endif