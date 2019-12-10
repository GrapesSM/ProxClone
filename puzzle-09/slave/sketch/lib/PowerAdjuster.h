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
    void show();
  private:
    ESP32Encoder *_encoder;
    Adafruit_7segment *_matrix;
    int _val;
    int _min;
    int _max;
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
  if (_val >= _max) {
    _val = _max;
    _encoder->setCount(_max);
  } else if (_val <= _min) {
    _val = _min;
    _encoder->setCount(_min);
  }
}

void PowerAdjuster::show() {
  _matrix->print(_val);
  _matrix->writeDisplay();
}

#endif