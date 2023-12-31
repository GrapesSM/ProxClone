/*
  CombinationReader.h - Library for ______.
*/
#ifndef CombinationReader_h
#define CombinationReader_h

#include <Arduino.h>

enum LIGHT_STATE {
  NO_COLOR,
  RED,
  YELLOW,
  GREEN
};

class CombinationReader
{
  public:
    CombinationReader();
    void init();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], ESP32Encoder * encoder);
    void setLightsRed();
    void setLightsYellow();
    void setLightGreen(int light);
    void setLightsOff();
    void checkNumber();
    void checkDirection();
    void update();
    void reset();
    void display();
    bool isSolved();
    void setState(STATE);
    int getValue();
    STATE getState();
  private:
    ESP32Encoder *_encoder;
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    bool _solved;
    bool _prepped;
    bool _clockwise;
    int _val;
    int _previousVal;
    int _submittedVal;
    int _min;
    int _max;
    int _increment;
    int _numbers[3] = {23, 7, 34};
    int _numbersSolved;
    LIGHT_STATE _lightState = NO_COLOR;
    LIGHT_STATE _prevLightState = NO_COLOR;
    const unsigned long _waitTimeMillis = 100; // ms
	  unsigned long lastRefreshTime;
    int overtravel;
    STATE _state;
};

CombinationReader::CombinationReader() 
{
  init();
}

void CombinationReader::init()
{
  _val = 0;
  _previousVal = 0;
  _min = 0;
  _max = 50;
  _submittedVal = 0;
  lastRefreshTime = 0;
  overtravel = 3;
  _prepped = false;
  _solved = false;
  _increment = 0;
  _numbersSolved = 0;
  _lightState = NO_COLOR;
  _prevLightState = NO_COLOR;
}

void CombinationReader::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, int lightPins[], ESP32Encoder *encoder) 
{
  _strip = strip;
  _lightPins = lightPins;
  _encoder = encoder;
}

void CombinationReader::setLightsRed()
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(255, 0, 0));
  _strip->SetPixelColor(_lightPins[1], RgbColor(255, 0, 0));
  _strip->SetPixelColor(_lightPins[2], RgbColor(255, 0, 0));
  _prevLightState = _lightState;
  _lightState = RED;
}

void CombinationReader::setLightsYellow()
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(255, 170, 0));
  _strip->SetPixelColor(_lightPins[1], RgbColor(255, 170, 0));
  _strip->SetPixelColor(_lightPins[2], RgbColor(255, 170, 0));
  _prevLightState = _lightState;
  _lightState = YELLOW;
}

void CombinationReader::setLightGreen(int light)
{
  _strip->SetPixelColor(light, RgbColor(0, 255, 0));
  _prevLightState = _lightState;
  _lightState = GREEN;
}

void CombinationReader::setLightsOff()
{
  _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
  _prevLightState = _lightState;
  _lightState = NO_COLOR;
}

void CombinationReader::checkNumber() 
{
  if (_submittedVal != _previousVal) {
    if (_submittedVal - _previousVal > 0) {     // changed clock states 
      _clockwise = true;
    } else {
      _clockwise = false;
    }

    if (_prepped) {
      switch (_numbersSolved) {
        case 0:
          if (_clockwise) {
            if (_submittedVal > 5) {
              reset();
            }
          } else {
            if (_submittedVal <= _numbers[_numbersSolved] + overtravel && _submittedVal >= _numbers[_numbersSolved] - overtravel) {
              _strip->SetPixelColor(_numbersSolved, RgbColor(0, 127, 0));
              _numbersSolved++;
            } else if (_submittedVal > _numbers[_numbersSolved] + overtravel) {
              reset();
            }
          }
        break;

        case 1:
          if (_clockwise) {
            if (_submittedVal <= _numbers[_numbersSolved] + overtravel && _submittedVal >= _numbers[_numbersSolved] - overtravel) {
              _strip->SetPixelColor(_numbersSolved, RgbColor(0, 127, 0));
              _numbersSolved++;
            } else if (_submittedVal < _numbers[_numbersSolved] - overtravel) {
              reset();
            }
          } else {
            reset();
          }
        break;

        case 2:
          if (_clockwise) {
            reset();
          } else {
            if (_submittedVal <= _numbers[_numbersSolved] + overtravel && _submittedVal >= _numbers[_numbersSolved] - overtravel) {
              _strip->SetPixelColor(_numbersSolved, RgbColor(0, 127, 0));
              _numbersSolved++;
              _solved = true;
              _encoder->pauseCount();
            } else if (_submittedVal > _numbers[_numbersSolved] + overtravel) {
              reset();
            }
          }
        break;

        default:

        break;
      }
    } else {
        if (_submittedVal < 5) {
          _prepped = true;
          setLightsYellow();
        } else {
          setLightsRed();
        }

        if (!_clockwise) {
          if (_submittedVal-_previousVal > 4) {
            _encoder->setCount(50);
          }
        }
    }
    _previousVal = _submittedVal;
  }
}

int CombinationReader::getValue()
{
  return _val;
}

void CombinationReader::update() 
{
  switch (_state)
  {
    case DISABLE:
      _encoder->pauseCount();
      setLightsOff();
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
      if (millis() - lastRefreshTime >= _waitTimeMillis) {
          lastRefreshTime = millis();
          _submittedVal = _val;
      }
      
      if (isSolved()) {
        _state = SOLVED;
      } else {
        checkNumber();
      }  
      break;
    
    default:
      break;
  }  
}

void CombinationReader::reset() 
{
  _increment = 0;
  _numbersSolved = 0;
  _prepped = false;
  _solved = false;
  _encoder->setCount(100);
  setLightsRed();
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