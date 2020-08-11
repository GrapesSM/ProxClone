/*
  PowerAdjuster.h - Library for playing sounds and voices.
*/
#ifndef PowerAdjuster_h
#define PowerAdjuster_h

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

class PowerAdjuster
{
  public:
    PowerAdjuster();
    void setDefaultValues();
    void set(ESP32Encoder * encoder, Adafruit_7segment * matrix);
    void update();
    void display();
    int getInputKey();
    STATE getState();
    void setState(STATE state);
  private:
    ESP32Encoder *_encoder;
    Adafruit_7segment *_matrix;
    int _val;
    int _checkVal;
    int _submittedVal;
    int _min;
    int _max;
    const unsigned long _waitTimeMillis = 3000; // ms
	  unsigned long lastRefreshTime;
    STATE _state;
};

PowerAdjuster::PowerAdjuster()
{
  _val = 0;
  _min = 600;
  _max = 700;
  _checkVal = -1;
  _submittedVal = 0;
  lastRefreshTime = 0;
}

int PowerAdjuster::getInputKey() 
{
  return _submittedVal;
}

void PowerAdjuster::setDefaultValues()
{
  _val = 0;
  _encoder->setCount(_val);
}

void PowerAdjuster::set(ESP32Encoder *encoder, Adafruit_7segment *matrix) 
{
  _encoder = encoder;
  _matrix = matrix;
}

void PowerAdjuster::update() 
{
  switch (_state) {
    case DISABLE:
      _encoder->pauseCount();
      _matrix->clear();
      break;
    
    case ENABLE:
      _encoder->resumeCount();
      _val = _encoder->getCount();
      if (_val >= _max) {
        _val = _max;
        _encoder->setCount(_max);
      } else if (_val <= _min) {
        _val = _min;
        _encoder->setCount(_min);
      }
      if (millis() - lastRefreshTime >= _waitTimeMillis){
        lastRefreshTime = millis();
        _submittedVal = _val;
      }
      break;
    default:
      _encoder->pauseCount();
      _matrix->clear();
      break;
  }
}

void PowerAdjuster::display() 
{
  _matrix->clear();
  if (_val > 0 && _state != DISABLE)
    _matrix->print(_val);
  _matrix->writeDisplay();
}

STATE PowerAdjuster::getState()
{
  return _state;
}

void PowerAdjuster::setState(STATE state)
{
  _state = state;
}

#endif