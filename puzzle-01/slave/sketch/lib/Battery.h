/*
  Battery.h - Library for ______.
*/
#ifndef Battery_h
#define Battery_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class Battery
{
  public:
    Battery();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[]);
    void init();
    void update();
    void disable();
    void enable();
    bool isDisabled();
    void setValue(float val);
    void setMaxValue(float maxValue);
    STATE getState();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    bool _disabled = true;
    float _value;
    float _maxValue;
    STATE _state;
};

Battery::Battery(){}

void Battery::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void Battery::init()
{
  Serial.println("Battery: Init");
  _state = INITIALIZED;
}

void Battery::update()
{
  int scale = map(_value, 0.0, _maxValue, 0, 7);
  for (int i = 0; i < scale; i++) {
    _strip->SetPixelColor(_lightPins[i], RgbColor(127, 0, 0));
  }

  for (int i = scale; i < 7; i++) {
    _strip->SetPixelColor(_lightPins[i], RgbColor(127, 127, 127));
  }
}

void Battery::disable() 
{
  _disabled = true;
  // TO-DO:
}

void Battery::enable() 
{
  _disabled = false;
  // TO-DO:
}

bool Battery::isDisabled()
{
  return _disabled;
}

void Battery::setValue(float value)
{
  _value = value;
}

void Battery::setMaxValue(float maxValue)
{
  _maxValue = maxValue;
}

STATE Battery::getState()
{
  return _state;
}


#endif