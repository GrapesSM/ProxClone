/*
  PowerBarIndicator.h - Library for ______.
*/
#ifndef PowerBarIndicator_h
#define PowerBarIndicator_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class PowerBarIndicator
{
  public:
    PowerBarIndicator();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[]);
    void update();
    void disable();
    void enable();
    bool isDisabled();
    void setValue(float val);
    void setMaxValue(float maxValue);
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    bool _disabled = true;
    float _value;
    float _maxValue;
};

PowerBarIndicator::PowerBarIndicator(){}

void PowerBarIndicator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void PowerBarIndicator::update()
{
  // 1 - 7
  int scale = map(_value, 0.0, _maxValue, 1.0, 7.0);
  if (scale != 7) {
    for (int i = 0; i < scale; i++) {
      _strip->SetPixelColor(_lightPins[i], RgbColor(127, 0, 0));
    }

    for (int i = scale; i < 7; i++) {
      _strip->SetPixelColor(_lightPins[i], RgbColor(127, 127, 127));
    }
  }
}

void PowerBarIndicator::disable() 
{
  _disabled = true;
  // TO-DO:
}

void PowerBarIndicator::enable() 
{
  _disabled = false;
  // TO-DO:
}

bool PowerBarIndicator::isDisabled()
{
  return _disabled;
}

void PowerBarIndicator::setValue(float value)
{
  _value = value;
}

void PowerBarIndicator::setMaxValue(float maxValue)
{
  _maxValue = maxValue;
}


#endif