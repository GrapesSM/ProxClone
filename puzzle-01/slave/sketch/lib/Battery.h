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
    float getValue();
    void setMaxValue(float maxValue);
    float getMaxValue();
    void setRate(float rate);
    float getRate();
    void setInterval(unsigned long milliseconds);
    unsigned long getInterval();
    void setDrawRate(float drawRate);
    STATE getState();
    void display();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    float _value;
    float _maxValue;
    float _rate;
    float _drawRate;
    STATE _state;
    struct ChargeTimer {
      unsigned long current;
      unsigned long chargepoint;
      unsigned long interval;
    } _chargeTimer;
};

Battery::Battery(){
  _value = 0;
  _rate = 1.00;
  _maxValue = 13.00;
  _chargeTimer.current = 0;
  _chargeTimer.chargepoint = 0;
  _chargeTimer.interval = 1000;
}

void Battery::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void Battery::setState(STATE state)
{
  _state = state;
}

STATE Battery::getState()
{
  return _state;
}

void Battery::update()
{
  if (_state == DISABLE) {
    return;
  }
  
  _state = CHARGING;
  _chargeTimer.current = millis();
  if ((_chargeTimer.current - _chargeTimer.chargepoint) > _chargeTimer.interval) {
    _chargeTimer.chargepoint = millis();
    _value += _rate;
    _value -= _drawRate;
    if (_value >= _maxValue) {
      _value = _maxValue;
    } else if (_value < 0) {
      _value = 0;
    }
  }
}

void Battery::setValue(float value)
{
  _value = value;
}

float Battery::getValue()
{
  return _value;
}

void Battery::setMaxValue(float maxValue)
{
  _maxValue = maxValue;
}

float Battery::getMaxValue()
{
  return _maxValue;
}

void Battery::setRate(float rate)
{
  _rate = rate;
}

float Battery::getRate()
{
  return _rate;
}

void Battery::setInterval(unsigned long milliseconds)
{
  _chargeTimer.interval = milliseconds;
}

unsigned long Battery::getInterval() 
{
  return _chargeTimer.interval;
}

void Battery::setDrawRate(float drawRate) 
{
  _drawRate = drawRate;
}

void Battery::display() 
{
  int scale;
  RgbColor color;
  switch (_state)
  {
    case DISABLE:
      for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_BAR_INDICATOR; i++) {  
        _strip->SetPixelColor(_lightPins[i], RgbColor(0,0,0));
      }
      break;
    
    case FULL:
    case EMPTY:
    case ENABLE:
    case CHARGING:
      scale = (int)map(_value, 0.0, _maxValue, 0.0, 7.0);
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
    
    default:
      break;
  }
}

#endif