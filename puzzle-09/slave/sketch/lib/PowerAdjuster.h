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

PowerAdjuster::PowerAdjuster() {
  _val = 0;
  _min = 0;
  _max = 100;
}

void PowerAdjuster::set(ESP32Encoder *encoder, Adafruit_7segment *matrix) {
  _encoder = encoder;
  _matrix = matrix;
}

void PowerAdjuster::update() {
  _val = _encoder->getCount();
  Serial.println(_val);
  if (_val >= _max) {
    _val = _max;
    _encoder->setCount(_max);
  } else if (_val <= _min) {
    _val = _min;
    _encoder->setCount(_min);
  }
}

void PowerAdjuster::disable() {
  _disabled = true;
  _encoder->pauseCount();
  _matrix->clear();
}

void PowerAdjuster::enable() {
  _disabled = false;
  _encoder->resumeCount();
}

bool PowerAdjuster::isDisabled()
{
  return _disabled;
}

void PowerAdjuster::display() {
  Serial.println(_val);
  _matrix->clear();
  _matrix->print(_val);
  _matrix->writeDisplay();
}

#endif