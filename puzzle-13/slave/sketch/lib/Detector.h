#ifndef Detector_h
#define Detector_h

#include <Arduino.h>
#include "Adafruit_VL53L0X.h"

class Detector
{
  public:
    Detector();
    void set(Adafruit_VL53L0X* lox, int pin);
    void update();
    void display();
    STATE getState();
    void setState(STATE);
  private:
    Adafruit_VL53L0X * _lox;
    int _pin;
    STATE _state;
};

Detector::Detector(){}

void Detector::set(Adafruit_VL53L0X* lox, int pin)
{
  _lox = lox;
  _pin = pin;
}

STATE Detector::getState() 
{
  return _state;
}

void Detector::setState(STATE state) {
  _state = state;
}

void Detector::update()
{
  if (_state == DISABLE) {
    return;
  }

  VL53L0X_RangingMeasurementData_t measure;
  _lox->rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    // Serial.println(measure.RangeMilliMeter);
    _state = DETECTED;
  } else {
    _state = OUT_OF_RANGE;
  }
    
  delay(100);
}

void Detector::display()
{
  if (_state == DISABLE) {
    digitalWrite(_pin, LOW);
    return;
  }

  digitalWrite(_pin, HIGH);
}
#endif