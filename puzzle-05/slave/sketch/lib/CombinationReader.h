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
    void setLightGreen(int light);
    void setLightsOff();
    void checkNumber();
    void checkDirection();
    void update();
    void display();
    void disable();
    void enable();
    bool isDisabled();
    bool isSolved();
  private:
    ESP32Encoder *_encoder;
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    bool _disabled = true;
    bool _solved;
    int _val;
    int _previousVal;
    int _submittedVal;
    int _min;
    int _max;
    int _numbers[3] = {23, 87, 45};
    int _numbersSolved = 0;
    const unsigned long _waitTimeMillis = 500; // ms
	  unsigned long lastRefreshTime;
    int overtravel;
};

CombinationReader::CombinationReader() {
  _val = 0;
  _previousVal = 0;
  _min = 0;
  _max = 100;
  _submittedVal = 0;
  lastRefreshTime = 0;
  overtravel = 2;
}

void CombinationReader::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, ESP32Encoder *encoder) {
  _strip = strip;

  _encoder = encoder;
}

void CombinationReader::setLightsRed()
{
  _strip->SetPixelColor(0, RgbColor(127, 0, 0));
  _strip->SetPixelColor(1, RgbColor(127, 0, 0));
  _strip->SetPixelColor(2, RgbColor(127, 0, 0));
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

void CombinationReader::checkNumber() {
  if (_submittedVal <= _numbers[_numbersSolved] + overtravel && _submittedVal >= _numbers[_numbersSolved] - overtravel) {
    if (_numbersSolved < 3) {
      _strip->SetPixelColor(_numbersSolved, RgbColor(0, 127, 0));
      _numbersSolved++;
      if (_numbersSolved == 3) {
        _solved = true;
        _encoder->pauseCount();
      }
    } 
  } 
}

void CombinationReader::checkDirection() {
  if(_numbersSolved == 1) {
    if(_previousVal < _submittedVal || _submittedVal > _numbers[_numbersSolved] + overtravel) {
      _numbersSolved = 0;
    }
  } else {
    if(_previousVal > _submittedVal || _submittedVal < _numbers[_numbersSolved] - overtravel) {
      _numbersSolved = 0;
    }
  }
  _previousVal = _submittedVal;
}

void CombinationReader::update() {
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
  //Serial.println(_val);
}

void CombinationReader::disable() {
  _disabled = true;
  _encoder->pauseCount();
  _strip->SetPixelColor(0, RgbColor(0, 0, 0));
  _strip->SetPixelColor(1, RgbColor(0, 0, 0));
  _strip->SetPixelColor(2, RgbColor(0, 0, 0));
  _encoder->pauseCount();
}

void CombinationReader::enable() {
  _disabled = false;
  _solved = false;
  _numbersSolved = 0;
  Serial.println("combination enabled");
  _encoder->resumeCount();
  // _strip->SetPixelColor(0, RgbColor(127, 0, 0));
  // _strip->SetPixelColor(1, RgbColor(127, 0, 0));
  // _strip->SetPixelColor(2, RgbColor(127, 0, 0));
  _encoder->setCount(100);
}

bool CombinationReader::isDisabled()
{
  return _disabled;
}

void CombinationReader::display() {
  // TO-DO
}

bool CombinationReader::isSolved() {
  return _solved;
}

#endif