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
    void setState(STATE state);
    void update();
    void setValue(float value);
    void setMaxValue(float maxValue);
    STATE getState();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
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

void Battery::setState(STATE state)
{
  _state = state;
}

void Battery::update()
{
  int scale;
  RgbColor color;
  switch (_state)
  {
    case ON:
      scale = map(_value, 0.0, 100.00, 7, 0);
      switch (scale)
      {
        case 7:
        case 6:
        case 5:
        case 4:
          color = RgbColor(0, 255, 0);
          break;
        case 3:
        case 2:
          color = RgbColor(255, 255, 0);
          break;
        case 1:
          color = RgbColor(255, 0, 0);
          break;
        default:
          break;
      }
      
      for (int i = 0; i < scale; i++) {  
        _strip->SetPixelColor(_lightPins[i], color);
      }

      for (int i = scale; i < NUMBER_OF_LIGHTS_FOR_BAR_INDICATOR; i++) {
        _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
      }
      break;
    
    case OFF:
      for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_BAR_INDICATOR; i++) {  
        _strip->SetPixelColor(_lightPins[i], RgbColor(0,0,0));
      }
      break;
    
    default:
      break;
  }
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