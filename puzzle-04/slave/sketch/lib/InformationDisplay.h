/*
  InformationDisplay.h - Library for playing sounds and voices.
*/
#ifndef InformationDisplay_h
#define InformationDisplay_h
#define RXD2 10
#define TXD2 13

#include <Arduino.h>
#include <ESP32Encoder.h>

class InformationDisplay
{
  public:
    InformationDisplay();
    void set(ESP32Encoder * encoder);
    void update();
    void display();
    void disable();
    void enable();
    bool isDisabled();
    void activate(bool activate);
  private:
    ESP32Encoder *_encoder;
    int _val;
    int _min;
    int _max;
    int _disabled = true;
    bool _activated = false;
};

InformationDisplay::InformationDisplay() {
  _val = 0;
  _min = 0;
  _max = 100;
}

void InformationDisplay::set(ESP32Encoder *encoder) {
  _encoder = encoder;
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); 
  Serial2.print("p0.pic=0");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}

void InformationDisplay::activate(bool activate = true){
  _activated = activate;
}

void InformationDisplay::update() {

  if(_activated){
      _val = _encoder->getCount();
    if (_val < _min) {
      _val = _max;
      _encoder->setCount(_val);
    }
    if(_val > _max){
      _val = _min; 
      _encoder->setCount(_val);
    }

    switch ((_val/10)%5) {
      case 0:
        Serial2.print("p0.pic=0");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        break;
      case 1:
        Serial2.print("p0.pic=1");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        break;
      case 2:
        Serial2.print("p0.pic=2");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        break;
      case 3:
        Serial2.print("p0.pic=3");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        break;
      case 4:
        Serial2.print("p0.pic=4");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        break;
      case 5:
        Serial2.print("p0.pic=5");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        break;      
    }
  }
}

void InformationDisplay::disable() {
  _disabled = true;
  _encoder->pauseCount();
}

void InformationDisplay::enable() {
  _disabled = false;
  _encoder->resumeCount();
}

bool InformationDisplay::isDisabled()
{
  return _disabled;
}

void InformationDisplay::display() {
  // Serial.println(_val);
}

#endif