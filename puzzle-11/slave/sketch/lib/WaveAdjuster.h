/*
  WaveAdjuster.h - Library for _______.
*/
#ifndef WaveAdjuster_h
#define WaveAdjuster_h

#include <Arduino.h>
#include "helpers.h"
#define EPS 10

class WaveAdjuster
{
  public:
    WaveAdjuster();
    void set(int inputPin1, int inputPin2, int inputPin3, HardwareSerial * serial);
    void update();
    void display();
    void disable();
    void enable();
    bool isSyncronized();
    bool isDisabled();
    int getValue(int number);
    STATE getState();
  private:
    int _inputPin1;
    int _inputPin2;
    int _inputPin3;
    HardwareSerial * _serial;
    int _disabled;
    unsigned long timer;
    unsigned long interval;
    int i, j, k;
    int _inputValues[3];
    int _offsetValues[3];
    STATE _state;
};

WaveAdjuster::WaveAdjuster() 
{
  _disabled = true;
  i = 0; j = 0; k = 0;
  timer = 0;
  interval = 0;
  _offsetValues[0] = 0;
  _offsetValues[1] = 123;
  _offsetValues[2] = 219;
}

void WaveAdjuster::set(int inputPin1, int inputPin2, int inputPin3, HardwareSerial * serial) {
  _inputPin1 = inputPin1;
  _inputPin2 = inputPin2;
  _inputPin3 = inputPin3;
  _serial = serial;
}

int WaveAdjuster::getValue(int number)
{
  int phase = map(_inputValues[number - 1], 0, 4096, _offsetValues[number - 1], _offsetValues[number - 1] + 359);
  return phase;
}

void WaveAdjuster::update() 
{
  if (_disabled) return;

  _state = READING;
  _inputValues[0] = analogRead(_inputPin1); 
  _inputValues[1] = analogRead(_inputPin2);
  _inputValues[2] = analogRead(_inputPin3);

  display();

  if (isSyncronized()) {
    _state = SOLVED;
  }
}

bool WaveAdjuster::isSyncronized()
{
  Serial.print(abs(getValue(1) - getValue(2)));
  Serial.print(",");
  Serial.print(abs(getValue(1) - getValue(3)));
  Serial.println();
  return (abs(_inputValues[0] - _inputValues[1]) < EPS) && (abs(_inputValues[1] - _inputValues[2]) < EPS);
}

void WaveAdjuster::disable() 
{
  _state = OFF;
  _disabled = true;
}

void WaveAdjuster::enable() 
{
  _state = ON;
  _disabled = false; 
}

bool WaveAdjuster::isDisabled()
{
  return _disabled;
}

void WaveAdjuster::display() 
{
  if (millis() - timer > interval) {
    timer = millis();
    i++;
    j++;
    k++;
  }
  
  int Value = map(waveValue(i, map(_inputValues[0], 0, 4096, _offsetValues[0], _offsetValues[0] + 359)), -150, 150, 10, 250);  //Read the pot value ann map it to 0.255 (max value of waveform=255)
  String Tosend = "add ";                                       //We send the string "add "
  Tosend += 1;                                                  //send the id of the block you want to add the value to
  Tosend += ",";
  Tosend += 0;                                                  //Channel of taht id, in this case channel 0 of the waveform
  Tosend += ",";
  Tosend += Value;                                              //Send the value and 3 full bytes
  Tosend += "\xFF\xFF\xFF";
  _serial->print(Tosend);
  
  int Value2 = map(waveValue(j, map(_inputValues[1], 0, 4096, _offsetValues[1], _offsetValues[1] + 359)), -150, 150, 10, 250);  //Read the pot value ann map it to 0.255 (max value of waveform=255)
  String Tosend2 = "add ";                                       //We send the string "add "
  Tosend2 += 1;                                                  //send the id of the block you want to add the value to
  Tosend2 += ",";
  Tosend2 += 1;                                                  //Channel of taht id, in this case channel 0 of the waveform
  Tosend2 += ",";
  Tosend2 += Value2;                                              //Send the value and 3 full bytes
  Tosend2 += "\xFF\xFF\xFF";
  _serial->print(Tosend2);
  
  int Value3 = map(waveValue(k,  map(_inputValues[2], 0, 4096, _offsetValues[2], _offsetValues[2] + 359)), -150, 150, 10, 250);  //Read the pot value ann map it to 0.255 (max value of waveform=255)
  String Tosend3 = "add ";                                       //We send the string "add "
  Tosend3 += 1;                                                  //send the id of the block you want to add the value to
  Tosend3 += ",";
  Tosend3 += 2;                                                  //Channel of taht id, in this case channel 0 of the waveform
  Tosend3 += ",";
  Tosend3 += Value3;                                              //Send the value and 3 full bytes
  Tosend3 += "\xFF\xFF\xFF";
  _serial->print(Tosend3);
  
  if (i >= 359) {
    i = 0;
  }

  if (j >= 359) {
    j = 0;
  }

  if (k >= 359) {
    k = 0;
  }  
}

STATE WaveAdjuster::getState()
{
  return _state;
}

#endif