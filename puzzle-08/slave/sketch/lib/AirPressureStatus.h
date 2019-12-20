/*
  AirPressureStatus.h - Library for playing sounds and voices.
*/
#ifndef AirPressureStatus_h
#define AirPressureStatus_h

#include <Arduino.h>

class AirPressureStatus
{
  public:
    AirPressureStatus();
    void set();
    void update();
    void disable();
    void enable();
    bool isDisabled();
    void display();
  private:
    bool _disabled = true;
};

AirPressureStatus::AirPressureStatus(){}

void AirPressureStatus::set()
{
  // TO-DO:
}

void AirPressureStatus::update()
{
  // TO-DO:
}

void AirPressureStatus::disable() 
{
  _disabled = true;
  // TO-DO:
}

void AirPressureStatus::enable() 
{
  _disabled = false;
  // TO-DO:
}

bool AirPressureStatus::isDisabled()
{
  return _disabled;
}

void AirPressureStatus::display()
{
  // TO-DO:
}

#endif