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
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[], Adafruit_MCP23017 *mcp);
    void update();
    void disable();
    void enable();
    bool isDisabled();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    Adafruit_MCP23017 * _mcp;
    bool _disabled = true;
};

BatteryMatrix::BatteryMatrix() {}

void BatteryMatrix::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[], Adafruit_MCP23017 *mcp)
{
  _strip = strip;
  _lightPins = lightPins;
  _mcp = mcp;
}

void BatteryMatrix::update()
{
  for (int i = 1; i <= NUMBER_OF_SWITCHES_1; i++) {
    if (_mcp->digitalRead(i)) {
      _strip->SetPixelColor(_lightPins[i-1], RgbColor(127, 127, 127));
    } else {
      _strip->SetPixelColor(_lightPins[i-1], RgbColor(0, 0, 0));
    }
  }
}

void BatteryMatrix::disable() 
{
  _disabled = true;
  for (int i = 1; i <= NUMBER_OF_SWITCHES_1; i++) {
    _strip->SetPixelColor(_lightPins[i-1], RgbColor(0, 0, 0));
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

#endif