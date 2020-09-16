/*
  AirPressureStatus.h - Library for playing sounds and voices.
*/
#ifndef AirPressureStatus_h
#define AirPressureStatus_h

#include <Arduino.h>

#define CHANGE_VALUE 100

class AirPressureStatus
{
  public:
    AirPressureStatus();
    void init();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], Adafruit_7segment *_matrix);
    void update();
    void increaseBy(int value);
    void decreaseBy(int value);
    void display();
    void setState(STATE state);
    int getValue();
    STATE getState();
  private:
    STATE _state;
    Adafruit_7segment * _matrix;
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int * _lightPins;
    int _value;
    int _prevValue;
};

AirPressureStatus::AirPressureStatus()
{
  init();
}

void AirPressureStatus::init()  
{
  _value = 0;
}

void AirPressureStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], Adafruit_7segment * matrix)
{
  _strip = strip;
  _lightPins = lightPins;
  _matrix = matrix;
}

void AirPressureStatus::update()
{
  switch (_state)
  {
    case DISABLE:
      _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
      _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
      break;
    
    case ENABLE:
    default:
      if ((_value / CHANGE_VALUE) == AIR_PRESSURE) {
        _state = BALANCED;
        _strip->SetPixelColor(_lightPins[0], RgbColor(255, 255, 255));
        _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
      } else {
        _state = UNBALANCED;
        _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
        _strip->SetPixelColor(_lightPins[1], RgbColor(255, 255, 255));
      }
      break;
  }  
}

void AirPressureStatus::increaseBy(int value)
{
  if ((_value + value) <= (150 * CHANGE_VALUE)) {
    _value = _value + value;
  } else {
    _value = 150 * CHANGE_VALUE;
  }
}

void AirPressureStatus::decreaseBy(int value)
{
  if ((_value - value) >= 0) {
    _value = _value - value;
  } else {
    _value = 0;
  }
}

void AirPressureStatus::setState(STATE state)
{
  _state = state;
}

STATE AirPressureStatus::getState()
{
  return _state;
}

int AirPressureStatus::getValue()
{
  return _value / CHANGE_VALUE;
}

void AirPressureStatus::display()
{
  switch (_state)
  {
    case DISABLE:
      _matrix->clear();
      _matrix->writeDisplay();
      break;
    
    case ENABLE:
    default:
      _matrix->clear();
      _matrix->print(_value / CHANGE_VALUE);
      _matrix->writeDisplay();
      break;
  }
}

#endif