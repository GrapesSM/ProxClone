/*
  Generator.h - Library for playing sounds and voices.
*/
#ifndef Generator_h
#define Generator_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class Generator
{
  public:
    Generator();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], Adafruit_MCP23017 *mcp, int switchPins[]);
    void update();
    void disable();
    void enable();
    bool isDisabled();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    int *_switchPins;
    Adafruit_MCP23017 *_mcp;
    bool _disabled = true;
};

Generator::Generator(){}

void Generator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], Adafruit_MCP23017 *mcp, int switchPins[])
{
  _strip = strip;
  _lightPins = lightPins;
  _mcp = mcp;
  _switchPins = switchPins;
}

void Generator::update()
{
  for (int i = 0; i < NUMBER_OF_SWITCHES_2; i++) {
    if (_mcp->digitalRead(_switchPins[i])) {
      _strip->SetPixelColor(_lightPins[i], RgbColor(127, 127, 127));
    } else {
      _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
    }
  }
}

void Generator::disable() 
{
  _disabled = true;
  for (int i = 0; i < NUMBER_OF_SWITCHES_2; i++) {
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


#endif