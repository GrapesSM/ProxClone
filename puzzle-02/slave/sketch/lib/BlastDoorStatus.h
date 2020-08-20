/*
  BlastDoorStatus.h - Library for ______.
*/
#ifndef BlastDoorStatus_h
#define BlastDoorStatus_h

#include <Arduino.h>

class BlastDoorStatus
{
  public:
    BlastDoorStatus();
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

BlastDoorStatus::BlastDoorStatus(){}

void BlastDoorStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void BlastDoorStatus::setState(STATE state)
{
  _state = state;
}

STATE BlastDoorStatus::getState()
{
  return _state;
}

void BlastDoorStatus::update()
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

void BlastDoorStatus::setRedLight(STATE state)
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

void BlastDoorStatus::setGreenLight(STATE state)
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

void BlastDoorStatus::display()
{
  _strip->Show();
}

#endif