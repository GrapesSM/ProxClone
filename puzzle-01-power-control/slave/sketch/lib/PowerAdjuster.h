/*
  PowerAdjuster.h - Library to control input and output of powers to others puzzles.
*/
#ifndef PowerAdjuster_h
#define PowerAdjuster_h

#include <Arduino.h>

class PowerAdjuster
{
  public:
    PowerAdjuster();
    void set(ESP32Encoder * encoder, Adafruit_7segment * matrix1, Adafruit_7segment * matrix2, int channel);
    void init();
    void update();
    void display();
    void setMaxSupply(float maxSupply);
    void setMaxDemand(float maxDemand);
    float getMaxSupply();
    float getMaxDemand();
    void setDemand(float demand);
    float getSupply();
    float getDemand();
    void setState(STATE state);
    STATE getState();
  private:
    ESP32Encoder *_encoder;
    Adafruit_7segment *_matrix1;
    Adafruit_7segment *_matrix2;
    float _supply;
    float _demand;
    float _maxSupply;
    int _channel;
    float _maxDemand;
    int _lastDialValue;
    STATE _state;
};

PowerAdjuster::PowerAdjuster() 
{
  init();
}

void PowerAdjuster::init()
{
  _supply = 0;
  _demand = 0;
  _channel = 0;
  _maxSupply = 0;
  _maxDemand = 0;
  _lastDialValue = 0;
}

void PowerAdjuster::set(ESP32Encoder * encoder, Adafruit_7segment * matrix1, Adafruit_7segment * matrix2, int channel) 
{
  _encoder = encoder;
  _matrix1 = matrix1;
  _matrix2 = matrix2;
  _channel = channel;
}

STATE PowerAdjuster::getState()
{
  return _state;
}

void PowerAdjuster::update() 
{
  switch (_state)
  {
    case DISABLE:
      _encoder->clearCount();
      _encoder->setCount((int)(_supply * 100));
      _encoder->pauseCount();  
      break;
    
    case RESET:
      _supply = 0;
      _demand = 0;
      _encoder->clearCount();
      _encoder->setCount((int)(_supply * 100));
      _encoder->pauseCount();  
      break;
    
    case ENABLE:
    default:
      _encoder->resumeCount();
      _supply = _encoder->getCount() / 100.00;
      if (_supply >= _maxSupply) {
        _supply = _maxSupply;
        _encoder->setCount((int)(_supply * 100));
      } else if (_supply <= 0) {
        _supply = 0;
        _encoder->setCount((int)(_supply * 100));
      }

      if (_supply == _demand) {
        _state = BALANCED;
      } else {
        _state = UNBALANCED;
      }
      break;
  } 
}

void PowerAdjuster::setState(STATE state) 
{
  _state = state;
}

void PowerAdjuster::display() 
{  
  _matrix1->clear();
  if (_state != DISABLE) 
    _matrix1->print(_supply);
  _matrix1->writeDisplay();

  _matrix2->clear();
  if (_state != DISABLE) 
    _matrix2->print(_demand);
  _matrix2->writeDisplay();
  
  if (_demand > _supply) {
    ledcWrite(_channel, 255);
  }
  else if (_demand < _supply) {
    ledcWrite(_channel, 0);
  }
  else if (_demand == _supply) {
    ledcWrite(_channel, 128);
  }
}

void PowerAdjuster::setMaxSupply(float maxSupply)
{
  _maxSupply = maxSupply;
}

void PowerAdjuster::setMaxDemand(float maxDemand)
{
  _maxDemand = maxDemand;
}

float PowerAdjuster::getSupply()
{
  return _supply;
}

float PowerAdjuster::getDemand() 
{
  return _demand;
}

float PowerAdjuster::getMaxSupply()
{
  return _maxSupply;
}

float PowerAdjuster::getMaxDemand()
{
  return _maxDemand;
}

void PowerAdjuster::setDemand(float demand) 
{
  _demand = demand;
}

#endif