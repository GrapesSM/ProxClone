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
    void update();
    void display();
    void disable();
    void enable();
    bool isDisabled();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int *_lightPins;
    int _val;
    int _min;
    int _max;
    int _disabled = true;
};

BlastDoorStatus::BlastDoorStatus() {
  _val = 0;
  _min = 0;
  _max = 100;
}

void BlastDoorStatus::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[]) {
  
}

void BlastDoorStatus::update() {
 
}

void BlastDoorStatus::disable() {
  _disabled = true;
  // TO-DO:
}

void BlastDoorStatus::enable() {
  _disabled = false;
  // TO-DO:
}

bool BlastDoorStatus::isDisabled()
{
  return _disabled;
}

void BlastDoorStatus::display() {  

}

#endif