/*
  ShipPrepStatus.h - Library for ______.
*/
#ifndef ShipPrepStatus_h
#define ShipPrepStatus_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class ShipPrepStatus
{
  public:
    ShipPrepStatus();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[]);
    void update();
    void disable();
    void enable();
    bool isDisabled();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    bool _disabled = true;
};

ShipPrepStatus::ShipPrepStatus(){}

void ShipPrepStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void ShipPrepStatus::update()
{
  // TO-DO:
}

void ShipPrepStatus::disable() 
{
  _disabled = true;
  // TO-DO:
}

void ShipPrepStatus::enable() 
{
  _disabled = false;
  // TO-DO:
}

bool ShipPrepStatus::isDisabled()
{
  return _disabled;
}


#endif