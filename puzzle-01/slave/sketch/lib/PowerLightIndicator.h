/*
  PowerLightIndicator.h - Library for ______.
*/
#ifndef PowerLightIndicator_h
#define PowerLightIndicator_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class PowerLightIndicator
{
  public:
    PowerLightIndicator();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin);
    void display();
    void setState(STATE state);
    STATE getState();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int _lightPin;
    STATE _state;
    unsigned long _flashTimeOn;
    unsigned long _flashTimeOff;
};

PowerLightIndicator::PowerLightIndicator() 
{
  _flashTimeOn = 0;
  _flashTimeOff = 0;
}

void PowerLightIndicator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin) 
{
  _strip = strip;
  _lightPin = lightPin;
  _state = OFF;
}

void PowerLightIndicator::setState(STATE state)
{
  _state = state;
}

void PowerLightIndicator::display()
{
  switch (_state)
  {
    case ON:
      _strip->SetPixelColor(_lightPin, RgbColor(127, 127, 127));    
      break;
    
    case OFF:
      _strip->SetPixelColor(_lightPin, RgbColor(0, 0, 0));
    
    case FLASH:
      if (millis() < _flashTimeOn) {
        _strip->SetPixelColor(_lightPin, RgbColor(255, 0, 0));
      } else if (millis() < _flashTimeOff) {
        _strip->SetPixelColor(_lightPin, RgbColor(0, 0, 0));
      } else {
        _flashTimeOn = millis() + 500;
        _flashTimeOff = millis() + 1000;  
      }
      break;

    default:
      break;
  }

  _strip->Show();
}

STATE PowerLightIndicator::getState()
{
  return _state;
}

#endif