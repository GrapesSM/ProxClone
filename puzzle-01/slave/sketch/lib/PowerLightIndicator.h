/*
  PowerLightIndicator.h - Library for ______.
*/
#ifndef PowerLightIndicator_h
#define PowerLightIndicator_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class PowerLightIndicator
{
  public:
    PowerLightIndicator();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin);
    void setLightOn();
    void setLightOff();
    void display();
    STATE _state;
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int _lightPin;
};

PowerLightIndicator::PowerLightIndicator() {}

void PowerLightIndicator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin) 
{
  _strip = strip;
  _lightPin = lightPin;
  _state = OFF;
}

void PowerLightIndicator::setLightOn()
{
  _strip->SetPixelColor(_lightPin, RgbColor(127, 127, 127));
}

void PowerLightIndicator::setLightOff()
{
  _strip->SetPixelColor(_lightPin, RgbColor(0, 0, 0));
}

void PowerLightIndicator::display()
{
  _strip->Show();
}

#endif