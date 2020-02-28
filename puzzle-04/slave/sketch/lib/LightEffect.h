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
    void display();
    void setState(STATE state);
    STATE getState();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    int _current;
    STATE _state;
};

LightEffect::LightEffect(){
  _current = 0;
}

void LightEffect::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void LightEffect::setState(STATE state)
{
  _state = state;
}

STATE LightEffect::getState()
{
  return _state;
}

void LightEffect::update()
{
  int next;
  switch (_state)
  {
    case ENABLE:
      next = _current + 1;
      if (next == NUMBER_OF_LIGHTS_FOR_LIGHT_EFFECT) {
        next = 0;
      }
      _strip->SetPixelColor(_lightPins[_current], RgbColor(0, 0, 0));
      _strip->SetPixelColor(_lightPins[next], RgbColor(HtmlColor((uint32_t)random(0, 16777216))));
      break;
    case DISABLE:
      for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_LIGHT_EFFECT; i++) {
        _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
      }
      break;
    case FLASH:
      next = _current + 1;
      if (next == NUMBER_OF_LIGHTS_FOR_LIGHT_EFFECT) {
        next = 0;
      }
      _strip->SetPixelColor(_lightPins[_current], RgbColor(0, 0, 0));
      _strip->SetPixelColor(_lightPins[next], RgbColor(255, 0, 0));
      break;
    default:
      break;
  }
}

void LightEffect::display()
{
  if (_state == DISABLE) {
    return;
  }
  _current++;
  if (_current == NUMBER_OF_LIGHTS_FOR_LIGHT_EFFECT) {
    _current = 0;
  }
}


#endif