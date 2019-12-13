/*
  AirSupplyPump.h - Library for playing sounds and voices.
*/
#ifndef AirSupplyPump_h
#define AirSupplyPump_h

#include <Arduino.h>

class AirSupplyPump
{
  public:
    AirSupplyPump();
    void set();
    void update();
    void disable();
    void enable();
    bool isDisabled();
  private:
    bool _disabled = true;
};

AirSupplyPump::AirSupplyPump(){}

void AirSupplyPump::set()
{
  // TO-DO:
}

void AirSupplyPump::update()
{
  // TO-DO:
}

void AirSupplyPump::disable() 
{
  _disabled = true;
  // TO-DO:
}

void AirSupplyPump::enable() 
{
  _disabled = false;
  // TO-DO:
}

bool AirSupplyPump::isDisabled()
{
  return _disabled;
}


#endif