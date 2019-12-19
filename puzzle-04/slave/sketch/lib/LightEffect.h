/*
  LightEffect.h - Library for _______.
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
    void setLightOn();
    void setLightOff();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
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

void LightEffect::setLightOn() 
{
  // TO-DO: 
}

void LightEffect::setLightOff() 
{
  // TO-DO: 
}

#endif