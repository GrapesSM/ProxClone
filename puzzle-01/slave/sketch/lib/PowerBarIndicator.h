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
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    bool _disabled = true;
    float _value;
};

PowerBarIndicator::PowerBarIndicator(){}

void PowerBarIndicator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void PowerBarIndicator::update()
{
  // TO-DO:
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


#endif