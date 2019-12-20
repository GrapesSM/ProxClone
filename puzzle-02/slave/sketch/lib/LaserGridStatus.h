/*
  LaserGridStatus.h - Library for ______.
*/
#ifndef LaserGridStatus_h
#define LaserGridStatus_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class LaserGridStatus
{
  public:
    LaserGridStatus();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin);
    void listen();
    void setRedLightOn();
    void setRedLightOff();
    void setGreenLightOn();
    void setGreenLightOff();
    bool isSolved();
    void display();
    STATE _state;
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int _lightPin;
    bool _solved;
};

LaserGridStatus::LaserGridStatus() {}

void LaserGridStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin) 
{
  _strip = strip;
  _lightPin = lightPin;
  _state = OFF;
}

void LaserGridStatus::listen()
{
  // TO-DO:
}

void LaserGridStatus::setRedLightOn()
{
  // TO-DO:
}

void LaserGridStatus::setRedLightOff()
{
  // TO-DO:
}

void LaserGridStatus::setRedLightOn()
{
  // TO-DO:
}

void LaserGridStatus::setRedLightOff()
{
  // TO-DO:
}

bool LaserGridStatus::isSolved()
{
  // TO-DO:
}

void LaserGridStatus::display()
{
  // TO-DO:
}

#endif