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
    int _disabled;
    float _input;
    float _output;
    bool _balanced;
};

PowerAdjuster::PowerAdjuster() {
  _input = 0;
  _output = 0;
  _disabled = true;
  _balanced = false;
}

void PowerAdjuster::set(ESP32Encoder * encoder, Adafruit_7segment * matrix1, Adafruit_7segment * matrix2) {
  _encoder = encoder;
  _matrix1 = matrix1;
  _matrix2 = matrix2;
}

void PowerAdjuster::update() {
  _input = random(0, _output + 1); // _encoder->getCount();
  if (_input >= _output) {
    _input = _output;
    // _encoder->setCount(_max);
  } else if (_input <= 0) {
    _input = 0;
    // _encoder->setCount(_min);
  }

  if (_input == _output) {
    _balanced == true;
  } else {
    _balanced = false;
  }
}

void PowerAdjuster::disable() {
  _disabled = true;
  _encoder->clearCount();
  _input = 0;
  _encoder->setCount(_input);
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

  
  ledcWrite(0, map(_input, 0, 7, 0, 1024));
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