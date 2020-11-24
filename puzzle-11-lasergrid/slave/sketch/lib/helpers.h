#ifndef helpers_h
#define helpers_h

#include <Arduino.h>

float waveValue(int degree, int phase = 0, int a = 150) {
  float radian = ((degree + phase) * PI) / 180;
  return a * sin(radian);
}

#endif