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
    void update();
    void disable();
    void enable();
    bool isDisabled();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    bool _disabled = true;
};

Generator::Generator(){}

void Generator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void Generator::update()
{
  // TO-DO:
}

void Generator::disable() 
{
  _disabled = true;
  // TO-DO:
}

void Generator::enable() 
{
  _disabled = false;
  // TO-DO:
}

bool Generator::isDisabled()
{
  return _disabled;
}


#endif