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
    void set(ESP32Encoder * encoder, Adafruit_7segment * matrix1, Adafruit_7segment * matrix2);
    void update();
    void display();
    void disable();
    void enable();
    bool isDisabled();
    bool isBalanced();
    float getInputValue();
    float getOutputValue();
    void setOutputValue(float output);
  private:
    ESP32Encoder *_encoder;
    Adafruit_7segment *_matrix1;
    Adafruit_7segment *_matrix2;
    int _val;
    int _min;
    int _max;
    int _disabled;
    float _input;
    float _output;
    bool _balanced;
};

PowerAdjuster::PowerAdjuster() {
  _val = 0;
  _min = 0;
  _max = 100;
  _disabled = true;
  _input = 0;
  _output = 0;
  _balanced = false;
}

void PowerAdjuster::set(ESP32Encoder * encoder, Adafruit_7segment * matrix1, Adafruit_7segment * matrix2) {
  _encoder = encoder;
  _matrix1 = matrix1;
  _matrix2 = matrix2;
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

  _input = _val;
  if (_input == _output) {
    _balanced == true;
  } else {
    _balanced = false;
  }
}

void PowerAdjuster::disable() {
  _disabled = true;
  _encoder->clearCount();
  _val = 0;
  _encoder->setCount(_val);
  _encoder->pauseCount();
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
  _matrix1->clear();
  _matrix1->print(_input);
  _matrix1->writeDisplay();

  _matrix2->clear();
  _matrix2->print(_output);
  _matrix2->writeDisplay();
}

bool PowerAdjuster::isBalanced() {
  return _balanced;
}

float PowerAdjuster::getInputValue() {
  return _input;
}

float PowerAdjuster::getOutputValue() {
  return _output;
}

void PowerAdjuster::setOutputValue(float output) {
  _output = output;
}

#endif