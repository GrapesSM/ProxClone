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
    void set(ESP32Encoder * encoder, Adafruit_7segment * matrix);
    void update();
    void display();
    void disable();
    void enable();
    bool isDisabled();
  private:
    ESP32Encoder *_encoder;
    Adafruit_7segment *_matrix;
    int _val;
    int _min;
    int _max;
    int _disabled = true;
};

CombinationReader::CombinationReader() {
  _val = 0;
  _min = 0;
  _max = 100;
}

void CombinationReader::set(ESP32Encoder *encoder, Adafruit_7segment *matrix) {
  _encoder = encoder;
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
}

void CombinationReader::disable() {
  _disabled = true;
  _encoder->pauseCount();
  // TO-DO:
}

void CombinationReader::enable() {
  _disabled = false;
  _encoder->resumeCount();
  // TO-DO
}

bool CombinationReader::isDisabled()
{
  return _disabled;
}

void CombinationReader::display() {
  // TO-DO
}

#endif