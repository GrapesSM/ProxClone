/*
  ExternalVent.h - Library for playing sounds and voices.
*/
#ifndef ExternalVent_h
#define ExternalVent_h

#include <Arduino.h>

class ExternalVent
{
  public:
    ExternalVent();
    void set();
    void update();
    void disable();
    void enable();
    bool isDisabled();
  private:
    bool _disabled = true;
};

ExternalVent::ExternalVent(){}

void ExternalVent::set()
{
  // TO-DO:
}

void ExternalVent::update()
{
  // TO-DO:
}

void ExternalVent::disable() 
{
  _disabled = true;
  // TO-DO:
}

void ExternalVent::enable() 
{
  _disabled = false;
  // TO-DO:
}

bool ExternalVent::isDisabled()
{
  return _disabled;
}


#endif