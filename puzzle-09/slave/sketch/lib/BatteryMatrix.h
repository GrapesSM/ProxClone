/*
  BatteryMatrix.h - Library for playing sounds and voices.
*/
#ifndef BatteryMatrix_h
#define BatteryMatrix_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class BatteryMatrix
{
  public:
    BatteryMatrix();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[], Adafruit_MCP23017 *mcp);
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    Adafruit_MCP23017 * _mcp;
};

BatteryMatrix::BatteryMatrix() {}

void BatteryMatrix::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[], Adafruit_MCP23017 *mcp)
{
  _strip = strip;
  _lightPins = lightPins;
  _mcp = mcp;
}

#endif