/*
  BlastDoorStatus.h - Library to control supply and demand of powers to others puzzles.
*/
#ifndef BlastDoorStatus_h
#define BlastDoorStatus_h

#include <Arduino.h>

class BlastDoorStatus
{
  public:
    BlastDoorStatus();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[]);
    void listen();
    void setRedLightOn();
    void setRedLightOff();
    void setGreenLightOn();
    void setGreenLightOff();
    bool isSolved();
    void display();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int *_lightPins;
    bool _solved;
};

BlastDoorStatus::BlastDoorStatus() {
  _val = 0;
  _min = 0;
  _max = 100;
}

void BlastDoorStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[]) 
{
  _strip = strip;
  _lightPins = lightPins;
  _solved = false;
}

void BlastDoorStatus::listen() 
{
  // TO-DO:
}

void BlastDoorStatus::setRedLightOn() 
{
  // TO-DO:
}

void BlastDoorStatus::setRedLightOff() 
{
  // TO-DO:
}

void BlastDoorStatus::setGreenLightOn()
{
  // TO-DO:
}

void BlastDoorStatus::setGreenLightOff()
{
  // TO-DO:
}

bool BlastDoorStatus::isSolved()
{
  return _solved;
}

void BlastDoorStatus::display() 
{  
  // TO-DO:
}

#endif