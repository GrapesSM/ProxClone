/*
  CombinationReader.h - Library for ______.
*/
#ifndef CombinationReader_h
#define CombinationReader_h

#include <Arduino.h>
#include <ESP32Encoder.h>

class CombinationReader
{
  public:
    CombinationReader();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, ESP32Encoder * encoder);
    void setLightsRed();
    void setLightsYellow();
    void setLightGreen(int light);
    void setLightsOff();
    void checkNumber();
    void checkDirection();
    void update();
    void reset();
    void display();
    void disable();
    void enable();
    bool isDisabled();
    bool isSolved();
    void setState(STATE);
    STATE getState();
  private:
    ESP32Encoder *_encoder;
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    bool _disabled = true;
    bool _solved;
    bool _prepped;
    bool _clockwise;
    int _val;
    int _previousVal;
    int _submittedVal;
    int _min;
    int _max;
    int _increment = 0;
    int _numbers[3] = {23, 7, 34};
    int _numbersSolved = 0;
    const unsigned long _waitTimeMillis = 100; // ms
	  unsigned long lastRefreshTime;
    int overtravel;
    STATE _state;
};

CombinationReader::CombinationReader() 
{
  _val = 0;
  _previousVal = 0;
  _min = 0;
  _max = 100;
  _submittedVal = 0;
  lastRefreshTime = 0;
  overtravel = 3;
  _prepped = false;
}

void CombinationReader::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, ESP32Encoder *encoder) 
{
  _strip = strip;
  _encoder = encoder;
}

void CombinationReader::setLightsRed()
{
  _strip->SetPixelColor(0, RgbColor(127, 0, 0));
  _strip->SetPixelColor(1, RgbColor(127, 0, 0));
  _strip->SetPixelColor(2, RgbColor(127, 0, 0));
}

void CombinationReader::setLightsYellow()
{
  _strip->SetPixelColor(0, RgbColor(64, 64, 0));
  _strip->SetPixelColor(1, RgbColor(64, 64, 0));
  _strip->SetPixelColor(2, RgbColor(64, 64, 0));
}

void CombinationReader::setLightGreen(int light)
{
  _strip->SetPixelColor(light, RgbColor(0, 127, 0));
}

void CombinationReader::setLightsOff()
{
  _strip->SetPixelColor(0, RgbColor(0, 0, 0));
  _strip->SetPixelColor(1, RgbColor(0, 0, 0));
  _strip->SetPixelColor(2, RgbColor(0, 0, 0));
}

void CombinationReader::checkNumber() 
{
  if (_submittedVal != _previousVal) {
    if (_submittedVal - _previousVal > 0) {
      _clockwise = false;
    } else {
      _clockwise = true;
    }

    if (_prepped) {
      switch (_numbersSolved) {
        case 0:
          if (_clockwise) {
            if (_submittedVal > 5) {
              CombinationReader::reset();
            }
          } else {
            if (_submittedVal <= _numbers[_numbersSolved] + overtravel && _submittedVal >= _numbers[_numbersSolved] - overtravel) {
              _strip->SetPixelColor(_numbersSolved, RgbColor(0, 127, 0));
              _numbersSolved++;
            } else if (_submittedVal > _numbers[_numbersSolved] + overtravel) {
              CombinationReader::reset();
            }
          }
        break;

        case 1:
          if (_clockwise) {
            if (_submittedVal <= _numbers[_numbersSolved] + overtravel && _submittedVal >= _numbers[_numbersSolved] - overtravel) {
              _strip->SetPixelColor(_numbersSolved, RgbColor(0, 127, 0));
              _numbersSolved++;
            } else if (_submittedVal < _numbers[_numbersSolved] - overtravel) {
              CombinationReader::reset();
            }
          } else {
            CombinationReader::reset();
          }
        break;

        case 2:
          if (_clockwise) {
            CombinationReader::reset();
          } else {
            if (_submittedVal <= _numbers[_numbersSolved] + overtravel && _submittedVal >= _numbers[_numbersSolved] - overtravel) {
              _strip->SetPixelColor(_numbersSolved, RgbColor(0, 127, 0));
              _numbersSolved++;
              _solved = true;
              _encoder->pauseCount();
            } else if (_submittedVal > _numbers[_numbersSolved] + overtravel) {
              CombinationReader::reset();
            }
          }
        break;

        default:

        break;
      }
    } else {
        if (_submittedVal < 5) {
          _prepped = true;
          CombinationReader::setLightsYellow();
          Serial.println("dial prepped");
        } else {
          CombinationReader::setLightsRed();
        }

        if (!_clockwise) {
          if (_submittedVal-_previousVal > 4){
            _encoder-> setCount(100);
          }
        }
    }
    _previousVal = _submittedVal;
  }
}

void CombinationReader::update() 
{
  if(_state == DISABLE) return;
  if(_state == ENABLE){
    _val = _encoder->getCount();
    if (_val >= _max) {
      _val = _max;
      _encoder->setCount(_max);
    } else if (_val <= _min) {
      _val = _min;
      _encoder->setCount(_min);
    }
    if(millis() - lastRefreshTime >= _waitTimeMillis){
        lastRefreshTime = millis();
        _submittedVal = _val;
    }
    Serial.println(_submittedVal);
    if(isSolved()){
      _state = SOLVED;
    }else
    {
      checkNumber();
    }
    
  }
}

void CombinationReader::reset() 
{
  _increment = 0;
  _numbersSolved = 0;
  _prepped = false;
  _solved = false;
  _encoder->setCount(100);
  CombinationReader::setLightsRed();
}

void CombinationReader::disable() 
{
  _disabled = true;
  _solved = false;
  _prepped = false;
  CombinationReader::setLightsOff();
  _encoder->pauseCount();
}

void CombinationReader::enable() 
{
  _disabled = false;
  _solved = false;
  _prepped = false;
  _numbersSolved = 0;
  _encoder->resumeCount();
  CombinationReader::setLightsRed();
  _encoder->setCount(100);
}

bool CombinationReader::isDisabled()
{
  return _disabled;
}

void CombinationReader::display() 
{
  // TO-DO
}

bool CombinationReader::isSolved() 
{
  return _solved;
}

STATE CombinationReader::getState()
{
  return _state;
}

void CombinationReader::setState(STATE state)
{
   _state = state;
}

#endif