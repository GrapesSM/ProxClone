/*
  LightEffect.h - Library for playing sounds and voices.
*/
#ifndef LightEffect_h
#define LightEffect_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class LightEffect
{
  public:
    LightEffect();
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

LightEffect::LightEffect(){}

void LightEffect::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void LightEffect::update()
{
  // TO-DO: 
}

void LightEffect::disable() 
{
  _disabled = true;
  // TO-DO: 
}

void LightEffect::enable() 
{
  _disabled = false;
  // TO-DO: 
}

bool LightEffect::isDisabled()
{
  return _disabled;
}


#endif