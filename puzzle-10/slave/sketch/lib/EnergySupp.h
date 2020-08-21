/*
  EnergySupp.h - Library for ________.
*/
#ifndef EnergySupp_h
#define EnergySupp_h

#include <Arduino.h>

class EnergySupp
{
  public:
    EnergySupp();
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

EnergySupp::EnergySupp() {}

void EnergySupp::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void EnergySupp::switchToRed() 
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[2], RgbColor(200, 200, 200));
}

void EnergySupp::switchToYellow() 
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[1], RgbColor(200, 200, 200));
  _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
}

void EnergySupp::switchToGreen() 
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(200, 200, 200));
  _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
}

void EnergySupp::update() 
{
  if (_state == DISABLE) {
    for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_ENERGY_SUPP; i++) {
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

void EnergySupp::setState(STATE state)
{
  _state = state;
}

STATE EnergySupp::getState()
{
  return _state;
}

void EnergySupp::display() 
{
  _strip->Show();
}

#endif