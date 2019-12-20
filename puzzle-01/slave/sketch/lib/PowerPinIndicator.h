/*
  PowerPinIndicator.h - Library for ______.
*/
#ifndef PowerPinIndicator_h
#define PowerPinIndicator_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class PowerPinIndicator
{
  public:
    PowerPinIndicator();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin);
    void setLightOn();
    void setLightOff();
    void display();
    STATE _state;
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int _lightPin;
};

PowerPinIndicator::PowerPinIndicator() {}

void PowerPinIndicator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin) 
{
  _strip = strip;
  _lightPin = lightPin;
  _state = OFF;
}

void PowerPinIndicator::setLightOn()
{
  _strip->SetPixelColor(_lightPin, RgbColor(127, 127, 127));
}

void PowerPinIndicator::setLightOff()
{
  _strip->SetPixelColor(_lightPin, RgbColor(0, 0, 0));
}

void PowerPinIndicator::display()
{
  _strip->Show();
}

#endif