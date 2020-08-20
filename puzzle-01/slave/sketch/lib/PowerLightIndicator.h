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
    void update();
    void setState(STATE state);
    STATE getState();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int _lightPin;
    STATE _state;
    int _flashCount;
    bool _flashToggle;
};

PowerLightIndicator::PowerLightIndicator() 
{
  _flashCount = 0;
  _flashToggle = false;
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

void PowerLightIndicator::update()
{
  switch (_state)
  {
    case ENABLE:
      _strip->SetPixelColor(_lightPin, RgbColor(127, 127, 127));    
      break;
    
    case DISABLE:
      _strip->SetPixelColor(_lightPin, RgbColor(0, 0, 0));
      break;
    
    case FAILURE:
      _strip->SetPixelColor(_lightPin, RgbColor(255,0,0));
      break;
      
    case FLASH:
      _flashCount++;
      if (_flashCount % 3 == 0) {
        _flashToggle = !_flashToggle;
      }

      if (_flashToggle) {
        _strip->SetPixelColor(_lightPin, RgbColor(255, 0, 0));
      } else  {
        _strip->SetPixelColor(_lightPin, RgbColor(127, 127, 127));
      }      
      break;

    default:
      break;
  }
}

void PowerLightIndicator::display()
{
   _strip->Show();
}

STATE PowerLightIndicator::getState()
{
  return _state;
}

#endif