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
    void display();
  private:
    bool _disabled = true;
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    int _current;
};

LightEffect::LightEffect(){}

void LightEffect::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
  _current = 0;
}

void LightEffect::update()
{
  int next = _current + 1;
  if (next == NUMBER_OF_LIGHTS_FOR_LIGHT_EFFECT) {
    next = 0;
  }
  _strip->SetPixelColor(_lightPins[_current], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[next], RgbColor(HtmlColor((uint32_t)random(0, 16777216))));
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

void LightEffect::display()
{
  _current++;
  if (_current == NUMBER_OF_LIGHTS_FOR_LIGHT_EFFECT) {
    _current = 0;
  }
}

#endif