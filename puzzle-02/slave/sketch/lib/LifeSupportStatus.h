/*
  LifeSupportStatus.h - Library for playing sounds and voices.
*/
#ifndef LifeSupportStatus_h
#define LifeSupportStatus_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class LifeSupportStatus
{
  public:
    LifeSupportStatus();
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

LifeSupportStatus::LifeSupportStatus(){}

void LifeSupportStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
  _solved = false;
}

void LifeSupportStatus::listen()
{
  // TO-DO: 
}

void LifeSupportStatus::setRedLightOn() 
{
  // TO-DO: 
}

void LifeSupportStatus::setRedLightOff() 
{
  // TO-DO: 
}

void LifeSupportStatus::setGreenLightOn()
{
  // TO-DO:
}

void LifeSupportStatus::setGreenLightOff()
{
  // TO-DO:
}

bool LifeSupportStatus::isSolved()
{
  return _solved;
}

void LifeSupportStatus::display()
{
  // TO-DO:
}

#endif