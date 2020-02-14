/*
  PowerAdjuster.h - Library to control input and output of powers to others puzzles.
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
    void set(ESP32Encoder * encoder, Adafruit_7segment * matrix1, Adafruit_7segment * matrix2, int channel);
    void init();
    void update();
    void display();
    void disable();
    void enable();
    bool isDisabled();
    bool isBalanced();
    float getSupplyValue();
    float getDemandValue();
    STATE getState();
    void setDemandValue(float demand);
  private:
    ESP32Encoder *_encoder;
    Adafruit_7segment *_matrix1;
    Adafruit_7segment *_matrix2;
    float _supply;
    float _demand;
    int _channel;
    bool _disabled;
    bool _balanced;
    STATE _state;
};

PowerAdjuster::PowerAdjuster() 
{
  _supply = 0;
  _demand = 0;
  _channel = 0;
  _disabled = true;
  _balanced = false;
}

void PowerAdjuster::set(ESP32Encoder * encoder, Adafruit_7segment * matrix1, Adafruit_7segment * matrix2, int channel) 
{
  _encoder = encoder;
  _matrix1 = matrix1;
  _matrix2 = matrix2;
  _channel = channel;
}

void PowerAdjuster::init()
{
  Serial.println("PowerAdjuster: Init");
  _state = INITIALIZED;
}

STATE PowerAdjuster::getState()
{
  return _state;
}

void PowerAdjuster::update() 
{
  _supply = random(0, _demand + 1); // _encoder->getCount();
  if (_supply >= _demand) {
    _supply = _demand;
    // _encoder->setCount(_max);
  } else if (_supply <= 0) {
    _supply = 0;
    // _encoder->setCount(_min);
  }

  if (_supply == _demand) {
    _balanced == true;
  } else {
    _balanced = false;
  }
}

void PowerAdjuster::disable() 
{
  _disabled = true;
  _encoder->clearCount();
  _supply = 0;
  _encoder->setCount(_supply);
  _encoder->pauseCount();
}

void PowerAdjuster::enable() 
{
  _disabled = false;
  _encoder->resumeCount();
}

bool PowerAdjuster::isDisabled()
{
  return _disabled;
}

void PowerAdjuster::display() 
{  
  _matrix1->clear();
  _matrix1->print(_supply);
  _matrix1->writeDisplay();

  _matrix2->clear();
  _matrix2->print(_demand);
  _matrix2->writeDisplay();
  
  ledcWrite(_channel, map(_supply, 0, 7, 0, 1024));
}

bool PowerAdjuster::isBalanced() 
{
  return _balanced;
}

float PowerAdjuster::getSupplyValue()
{
  return _supply;
}

float PowerAdjuster::getDemandValue() 
{
  return _demand;
}

void PowerAdjuster::setDemandValue(float demand) 
{
  _demand = demand;
}

#endif