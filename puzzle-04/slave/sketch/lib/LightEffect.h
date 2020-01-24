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
    void enable();
    void disable();
    bool isDisabled();
    void display();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    int _current;
    bool _disabled;
};

LightEffect::LightEffect(){}

void LightEffect::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
  _current = 0;
  _disabled = true;
}

void LightEffect::update()
{
  int next = _current + 1;
  if (next == NUMBER_OF_LIGHTS_FOR_LIGHT_EFFECT) {
    next = 0;
  }
  _strip->SetPixelColor(_lightPins[_current], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[next], RgbColor(127, 0, 0));
}

void LightEffect::enable()
{
  _disabled = false;
  // TO-DO:
}

void LightEffect::disable()
{
  _disabled = true;
}

bool LightEffect::isDisabled()
{
  return _disabled;
}

void LightEffect::display()
{
  _current++;
  if (_current == NUMBER_OF_LIGHTS_FOR_LIGHT_EFFECT) {
    _current = 0;
  }
}

#endif