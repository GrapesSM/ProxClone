/*
  WaveAdjuster.h - Library for _______.
*/
#ifndef WaveAdjuster_h
#define WaveAdjuster_h

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

class WaveAdjuster
{
  public:
    WaveAdjuster();
    void set(ESP32Encoder * encoder1, ESP32Encoder * encoder2, ESP32Encoder * encoder3, Adafruit_7segment * matrix);
    void update();
    void display();
    void disable();
    void enable();
    bool isDisabled();
  private:
    ESP32Encoder *_encoder1;
    ESP32Encoder *_encoder2;
    ESP32Encoder *_encoder3;
    Adafruit_7segment *_matrix;
    int _val;
    int _min;
    int _max;
    int _disabled;
};

WaveAdjuster::WaveAdjuster() {
  _val = 0;
  _min = 0;
  _max = 100;
}

void WaveAdjuster::set(ESP32Encoder * encoder1, ESP32Encoder * encoder2, ESP32Encoder * encoder3, Adafruit_7segment * matrix) {
  _encoder1 = encoder1;
  _encoder2 = encoder2;
  _encoder3 = encoder3;
  _matrix = matrix;
  _disabled = true;
}

void WaveAdjuster::update() {
  _val = _encoder->getCount();
  if (_val >= _max) {
    _val = _max;
    _encoder->setCount(_max);
  } else if (_val <= _min) {
    _val = _min;
    _encoder->setCount(_min);
  }
}

void WaveAdjuster::disable() {
  _disabled = true;
  _encoder->pauseCount();
  _matrix->clear();
}

void WaveAdjuster::enable() {
  _disabled = false;
  _encoder->resumeCount();
}

bool WaveAdjuster::isDisabled()
{
  return _disabled;
}

void WaveAdjuster::display() {
  Serial.println(_val);
  _matrix->clear();
  _matrix->print(_val);
  _matrix->writeDisplay();
}

#endif