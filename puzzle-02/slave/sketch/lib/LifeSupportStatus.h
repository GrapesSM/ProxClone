/*
  LifeSupportStatus.h - Library for playing sounds and voices.
*/
#ifndef LifeSupportStatus_h
#define LifeSupportStatus_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class LifeSupportStatus
{
  public:
    LifeSupportStatus();
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

LifeSupportStatus::LifeSupportStatus(){}

void LifeSupportStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void LifeSupportStatus::update()
{
  // TO-DO: 
}

void LifeSupportStatus::disable() 
{
  _disabled = true;
  // TO-DO: 
}

void LifeSupportStatus::enable() 
{
  _disabled = false;
  // TO-DO: 
}

bool LifeSupportStatus::isDisabled()
{
  return _disabled;
}


#endif