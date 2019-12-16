/*
  EnergySupplemental.h - Library for playing sounds and voices.
*/
#ifndef EnergySupplemental_h
#define EnergySupplemental_h

#include <Arduino.h>

class EnergySupplemental
{
  public:
    EnergySupplemental();
    void set();
    void update();
    void disable();
    void enable();
    bool isDisabled();
  private:
    bool _disabled = true;
};

EnergySupplemental::EnergySupplemental() 
{
  // TO-DO:
}

void EnergySupplemental::set() 
{ 
  // TO-DO:
}

void EnergySupplemental::update() 
{
  // TO-DO:
}

void EnergySupplemental::disable() 
{
  _disabled = true;
  // TO-DO: 
}

void EnergySupplemental::enable() 
{
  _disabled = false;
  // TO-DO:
}

bool EnergySupplemental::isDisabled()
{
  return _disabled;
}

#endif