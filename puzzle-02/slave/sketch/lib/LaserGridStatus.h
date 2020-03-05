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
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[]);
    void update();
    void setRedLight(STATE state);
    void setGreenLight(STATE state);
    void setState(STATE state);
    STATE getState();
    void display();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    STATE _state;
};

LaserGridStatus::LaserGridStatus(){}

void LaserGridStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void LaserGridStatus::setState(STATE state)
{
  _state = state;
}

STATE LaserGridStatus::getState()
{
  return _state;
}

void LaserGridStatus::update()
{
  switch (_state)
  {
    case DISABLE:
      _strip->SetPixelColor(_lightPins[0], RgbColor(0,0,0));
      _strip->SetPixelColor(_lightPins[1], RgbColor(0,0,0));
      break;
  
    case ENABLE:
    default:
      break;
  }
}

void LaserGridStatus::setRedLight(STATE state)
{
  switch (state)
  {
    case ON:
      _strip->SetPixelColor(_lightPins[0], RgbColor(255,255,255));
      break;
  
    case OFF:
    default:
      _strip->SetPixelColor(_lightPins[0], RgbColor(0,0,0));
      break;
  }
}

void LaserGridStatus::setGreenLight(STATE state)
{
  switch (state)
  {
    case ON:
      _strip->SetPixelColor(_lightPins[1], RgbColor(255,255,255));
      break;
  
    case OFF:
    default:
      _strip->SetPixelColor(_lightPins[1], RgbColor(0,0,0));
      break;
  }
}

void LaserGridStatus::display()
{
  _strip->Show();
}

#endif