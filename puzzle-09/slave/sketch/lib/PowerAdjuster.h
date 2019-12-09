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
  private:
    ESP32Encoder *_encoder;
    Adafruit_7segment *_matrix;
};

PowerAdjuster::PowerAdjuster() {}

void PowerAdjuster::set(ESP32Encoder *encoder, Adafruit_7segment *matrix) {
  _encoder = encoder;
  _matrix = _matrix;
}

#endif