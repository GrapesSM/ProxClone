/*
  EnergySupp.h - Library for ______.
*/
#ifndef EnergySupp_h
#define EnergySupp_h

#include <Arduino.h>

class EnergySupp
{
  public:
    EnergySupp();
    void set();
    void update();
    void disable();
    void enable();
    bool isDisabled();
  private:
    bool _disabled = true;
};

EnergySupp::EnergySupp() 
{
  // TO-DO:
}

void EnergySupp::set() 
{ 
  // TO-DO:
}

void EnergySupp::update() 
{
  // TO-DO:
}

void EnergySupp::disable() 
{
  _disabled = true;
  // TO-DO: 
}

void EnergySupp::enable() 
{
  _disabled = false;
  // TO-DO:
}

bool EnergySupp::isDisabled()
{
  return _disabled;
}

#endif