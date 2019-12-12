/*
  PowerIndicator.h - Library for ______.
*/
#ifndef PowerIndicator_h
#define PowerIndicator_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class PowerIndicator
{
  public:
    PowerIndicator();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin);
    void setLightOn();
    void setLightOff();
    void display();
    STATE _state;
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int _lightPin;
};

PowerIndicator::PowerIndicator() {}

void PowerIndicator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin) 
{
  _strip = strip;
  _lightPin = lightPin;
  _state = OFF;
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