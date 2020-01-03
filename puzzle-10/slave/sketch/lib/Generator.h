/*
  Generator.h - Library for ______.
*/
#ifndef Generator_h
#define Generator_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class Generator
{
  public:
    Generator();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[]);
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

Generator::Generator(){}

void Generator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
  _disabled = true;
}

void Generator::switchToRed() 
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[2], RgbColor(200, 200, 200));
}

void Generator::switchToYellow() 
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[1], RgbColor(200, 200, 200));
  _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
}

void Generator::switchToGreen() 
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(200, 200, 200));
  _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
}

void Generator::disable() 
{
  _disabled = true;
  for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_GENERATOR; i++) {
    _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
  }
}

void Generator::enable() 
{
  _disabled = false;
}

bool Generator::isDisabled()
{
  return _disabled;
}

void Generator::display() 
{
  _strip->Show();
}


#endif