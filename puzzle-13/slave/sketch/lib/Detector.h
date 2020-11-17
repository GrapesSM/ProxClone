#ifndef Detector_h
#define Detector_h

#include <Arduino.h>

class Detector
{
  public:
    Detector();
    void init();
    void set(Adafruit_VL53L0X & lox, int pin);
    void update();
    void display();
    uint16_t getMeasure();
    STATE getState();
    void setState(STATE);
  private:
    Adafruit_VL53L0X _lox;
    uint16_t _measure;
    int _pin;
    STATE _state;
};

Detector::Detector()
{
  init();
}

void Detector::init()
{

}

void Detector::set(Adafruit_VL53L0X & lox, int pin)
{
  _lox = lox;
  _pin = pin;
}

uint16_t Detector::getMeasure()
{
  return _measure;
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
  // if (_state == DISABLE) {
  //   return;
  // }

  VL53L0X_RangingMeasurementData_t measure;
  _lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  _measure = measure.RangeMilliMeter;
  if (measure.RangeStatus != 4) {
    if (measure.RangeMilliMeter > 5 && measure.RangeMilliMeter < 1500) {  // phase failures have incorrect data
      _state = DETECTED;
    } else {
      _state = ENABLE;
    }
  }
  else {
    _state = ENABLE;
  }
    
  delay(100);
}

void Detector::display()
{
  switch (_state)
  {
  case DISABLE:
    digitalWrite(_pin, LOW);
    break;

  case ENABLE:
    digitalWrite(_pin, HIGH);
    break;
  
  default:
    break;
  }
}
#endif