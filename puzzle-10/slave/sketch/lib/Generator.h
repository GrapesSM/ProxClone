/*
  Generator.h - Library for ________.
*/
#ifndef Generator_h
#define Generator_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class Generator
{
  public:
    Generator();
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

Generator::Generator() {}

void Generator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
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

void Generator::update() 
{
  if (_state == DISABLE) {
    for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_GENERATOR; i++) {
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

void Generator::setState(STATE state)
{
  _state = state;
}

STATE Generator::getState()
{
  return _state;
}

void Generator::display() 
{
  _strip->Show();
}

#endif