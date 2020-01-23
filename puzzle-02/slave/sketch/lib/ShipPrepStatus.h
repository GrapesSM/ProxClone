/*
  ShipPrepStatus.h - Library for ______.
*/
#ifndef ShipPrepStatus_h
#define ShipPrepStatus_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class ShipPrepStatus
{
  public:
    ShipPrepStatus();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[]);
    void listen();
    void setRedLightOn();
    void setRedLightOff();
    void setGreenLightOn();
    void setGreenLightOff();
    bool isSolved();
    void display();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    bool _solved;
};

ShipPrepStatus::ShipPrepStatus(){}

void ShipPrepStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
  _solved = false;
}

void ShipPrepStatus::listen()
{
  // TO-DO:
}

void ShipPrepStatus::setRedLightOn()
{
  // TO-DO:
}

void ShipPrepStatus::setRedLightOff()
{
  // TO-DO:
}

void ShipPrepStatus::setGreenLightOn()
{
  // TO-DO:
}

void ShipPrepStatus::setGreenLightOff()
{
  // TO-DO:
}

bool ShipPrepStatus::isSolved()
{
  return _solved;
}

void ShipPrepStatus::display()
{
  // TO-DO:
}

#endif