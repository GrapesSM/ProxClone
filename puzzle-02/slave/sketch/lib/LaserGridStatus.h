/*
  LaserGridStatus.h - Library for ______.
*/
#ifndef LaserGridStatus_h
#define LaserGridStatus_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class LaserGridStatus
{
  public:
    LaserGridStatus();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin);
    void setLightOn();
    void setLightOff();
    void display();
    STATE _state;
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int _lightPin;
};

LaserGridStatus::LaserGridStatus() {}

void LaserGridStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin) 
{
  _strip = strip;
  _lightPin = lightPin;
  _state = OFF;
}

void LaserGridStatus::setLightOn()
{
  _strip->SetPixelColor(_lightPin, RgbColor(127, 127, 127));
}

void LaserGridStatus::setLightOff()
{
  _strip->SetPixelColor(_lightPin, RgbColor(0, 0, 0));
}

void LaserGridStatus::display()
{
  _strip->Show();
}

#endif