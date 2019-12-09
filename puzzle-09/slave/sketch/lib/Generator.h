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
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], Adafruit_MCP23017 *mcp);
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    Adafruit_MCP23017 *_mcp;
};

Generator::Generator(){}
void Generator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], Adafruit_MCP23017 *mcp)
{
  _strip = strip;
  _lightPins = lightPins;
  _mcp = mcp;
}

#endif