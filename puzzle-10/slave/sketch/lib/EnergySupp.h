/*
  EnergySupp.h - Library for ______.
*/
#ifndef EnergySupp_h
#define EnergySupp_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class EnergySupp
{
  public:
    EnergySupp();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[]);\
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

EnergySupp::EnergySupp() {}

void EnergySupp::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
  _disabled = true;
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

void EnergySupp::disable() 
{
  _disabled = true;
  for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_ENERGY_SUPP; i++) {
    _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
  }
}

void EnergySupp::enable() 
{
  _disabled = false;
}

bool EnergySupp::isDisabled()
{
  return _disabled;
}

void EnergySupp::display()
{
  _strip->Show();
}

#endif