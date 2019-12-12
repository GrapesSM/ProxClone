/*
  PowerControl.h - Library to control supply and demand of powers to others puzzles.
*/
#ifndef PowerControl_h
#define PowerControl_h

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

class PowerControl
{
  public:
    PowerControl();
    void set(ESP32Encoder * encoder, Adafruit_7segment * matrix1, Adafruit_7segment * matrix2, void * dial);
    void update();
    void display();
    void disable();
    void enable();
    bool isDisabled();
  private:
    ESP32Encoder *_encoder;
    Adafruit_7segment *_matrix1;
    Adafruit_7segment *_matrix2;
    int _val;
    int _min;
    int _max;
    int _disabled = true;
};

PowerControl::PowerControl() {
  _val = 0;
  _min = 0;
  _max = 100;
}

void PowerControl::set(ESP32Encoder * encoder, Adafruit_7segment * matrix1, Adafruit_7segment * matrix2, void * dial) {
  _encoder = encoder;
  _matrix1 = matrix1;
  _matrix2 = matrix2;
}

void PowerControl::update() {
  _val = _encoder->getCount();
  if (_val >= _max) {
    _val = _max;
    _encoder->setCount(_max);
  } else if (_val <= _min) {
    _val = _min;
    _encoder->setCount(_min);
  }
}

void PowerControl::disable() {
  _disabled = true;
  _encoder->pauseCount();
  // TO-DO:
}

void PowerControl::enable() {
  _disabled = false;
  _encoder->resumeCount();
  // TO-DO:
}

bool PowerControl::isDisabled()
{
  return _disabled;
}

void PowerControl::display() {  
  _matrix1->clear();
  _matrix1->print(_val);
  _matrix1->writeDisplay();

  _matrix2->clear();
  _matrix2->print(_val);
  _matrix2->writeDisplay();
}

#endif