/*
  ShipPrepStatus.h - Library for ______.
*/
#ifndef ShipPrepStatus_h
#define ShipPrepStatus_h

#include <Arduino.h>

class ShipPrepStatus
{
  public:
    ShipPrepStatus();
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

ShipPrepStatus::ShipPrepStatus()
{
  init();
}

void ShipPrepStatus::init()
{

}

void ShipPrepStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void ShipPrepStatus::setState(STATE state)
{
  _state = state;
}

STATE ShipPrepStatus::getState()
{
  return _state;
}

void ShipPrepStatus::update()
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

void ShipPrepStatus::setRedLight(STATE state)
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

void ShipPrepStatus::setGreenLight(STATE state)
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

void ShipPrepStatus::display()
{
  _strip->Show();
}

#endif