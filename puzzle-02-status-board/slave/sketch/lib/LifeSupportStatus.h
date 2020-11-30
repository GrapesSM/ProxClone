/*
  LifeSupportStatus.h - Library for ______.
*/
#ifndef LifeSupportStatus_h
#define LifeSupportStatus_h

#include <Arduino.h>

class LifeSupportStatus
{
  public:
    LifeSupportStatus();
    void init();
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

LifeSupportStatus::LifeSupportStatus()
{
  init();
}

void LifeSupportStatus::init()
{
  
}

void LifeSupportStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void LifeSupportStatus::setState(STATE state)
{
  _state = state;
}

STATE LifeSupportStatus::getState()
{
  return _state;
}

void LifeSupportStatus::update()
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

void LifeSupportStatus::setRedLight(STATE state)
{
  switch (state)
  {
    case ON:
      _strip->SetPixelColor(_lightPins[0], RgbColor(255,0,0));
      break;
  
    case OFF:
    default:
      _strip->SetPixelColor(_lightPins[0], RgbColor(0,0,0));
      break;
  }
}

void LifeSupportStatus::setGreenLight(STATE state)
{
  switch (state)
  {
    case ON:
      _strip->SetPixelColor(_lightPins[1], RgbColor(0,255,0));
      break;
  
    case OFF:
    default:
      _strip->SetPixelColor(_lightPins[1], RgbColor(0,0,0));
      break;
  }
}

void LifeSupportStatus::display()
{
  _strip->Show();
}

#endif