/*
  BatteryMatrix.h - Library for ________.
*/
#ifndef BatteryMatrix_h
#define BatteryMatrix_h

#include <Arduino.h>

class BatteryMatrix
{
  public:
    BatteryMatrix();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[]);
    void switchToRed(); 
    void switchToYellow();
    void switchToGreen();
    void display();
    void update();
    void setState(STATE state);
    STATE getState();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    STATE _state;
};

BatteryMatrix::BatteryMatrix() {}

void BatteryMatrix::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void BatteryMatrix::switchToRed() 
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[2], RgbColor(200, 200, 200));
}

void BatteryMatrix::switchToYellow() 
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[1], RgbColor(200, 200, 200));
  _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
}

void BatteryMatrix::switchToGreen() 
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(200, 200, 200));
  _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
}

void BatteryMatrix::update() 
{
  if (_state == DISABLE) {
    for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_BATTERY_MATRIX; i++) {
      _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
    }
    return;
  }

  if (_state == SOLVED) {
    switchToYellow();
  } else {
    switchToRed();
  }
}

void BatteryMatrix::setState(STATE state)
{
  _state = state;
}

STATE BatteryMatrix::getState()
{
  return _state;
}

void BatteryMatrix::display() 
{
  _strip->Show();
}

#endif