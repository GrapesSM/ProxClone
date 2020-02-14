/*
  LightEffect.h - Library for ______.
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
    void init();
    void update();
    void enable();
    void disable();
    bool isDisabled();
    void display();
    STATE getState();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    int _current;
    bool _disabled;
    STATE _state;
};

LightEffect::LightEffect(){
  _disabled = true;
  _current = 0;
}

void LightEffect::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void LightEffect::init()
{
  Serial.println("LightEffect: Init");
  _state = INITIALIZED;
}

STATE LightEffect::getState()
{
  return _state;
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