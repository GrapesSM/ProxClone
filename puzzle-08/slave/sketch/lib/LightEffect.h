/*
  LightEffect.h - Library for playing sounds and voices.
*/
#ifndef LightEffect_h
#define LightEffect_h

#include <Arduino.h>

class LightEffect
{
  public:
    LightEffect();
    void set();
    void update();
    void disable();
    void enable();
    bool isDisabled();
  private:
    bool _disabled = true;
};

LightEffect::LightEffect(){}

void LightEffect::set()
{
  // TO-DO:
}

void LightEffect::update()
{
  // TO-DO:
}

void LightEffect::disable() 
{
  _disabled = true;
  // TO-DO:
}

void LightEffect::enable() 
{
  _disabled = false;
  // TO-DO:
}

bool LightEffect::isDisabled()
{
  return _disabled;
}


#endif