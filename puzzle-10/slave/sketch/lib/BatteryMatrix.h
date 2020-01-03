/*
  BatteryMatrix.h - Library for ________.
*/
#ifndef BatteryMatrix_h
#define BatteryMatrix_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class BatteryMatrix
{
  public:
    BatteryMatrix();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[]);
    void switchToRed(); 
    void switchToYellow();
    void switchToGreen();
    void disable();
    void enable();
    void display();
    bool isDisabled();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    bool _disabled;
};

BatteryMatrix::BatteryMatrix() {}

void BatteryMatrix::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
  _disabled = true;
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

void BatteryMatrix::disable() 
{
  _disabled = true;
  for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_BATTERY_MATRIX; i++) {
    _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
  }
}

void BatteryMatrix::enable() 
{
  _disabled = false;
}

bool BatteryMatrix::isDisabled()
{
  return _disabled;
}

void BatteryMatrix::display() 
{
  _strip->Show();
}

#endif