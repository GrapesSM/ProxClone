/*
  InformationDisplay.h - Library for playing sounds and voices.
*/
#ifndef InformationDisplay_h
#define InformationDisplay_h
#define RXD2 10
#define TXD2 13

#include <Arduino.h>

class InformationDisplay
{
  public:
    InformationDisplay();
    void init();
    void set(ESP32Encoder * encoder);
    void update();
    void display();
    void frameShowSeries();
    void setCurrentSeries(int);
    STATE getState();
    void setState(STATE state);
  private:
    ESP32Encoder *_encoder;
    int _val;
    int _min;
    int _max;
    int _currentSeries;
    STATE _state;
};

InformationDisplay::InformationDisplay() {
  init();
}

void InformationDisplay::init()
{
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
  _currentSeries = 0;
}
void InformationDisplay::setCurrentSeries(int current){
  _currentSeries = current;
}

void InformationDisplay::update() {
  if(_state == DISABLE) {
    _encoder->pauseCount();
    return;
  }
  
  if(_state == ENABLE) {
    _encoder->resumeCount();
    _val = _encoder->getCount();
    if (_val < _min) {
      _val = _max;
      _encoder->setCount(_val);
    }
    if(_val > _max){
      _val = _min; 
      _encoder->setCount(_val);
    }
  }
}

void InformationDisplay::display() {
  switch (_currentSeries) {
    case 0:
      Serial2.print("p0.pic=0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      break;
    case 1:
      Serial2.print("p0.pic=2");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      break;
    case 2:
      Serial2.print("p0.pic=3");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      break;
    case 3:
      Serial2.print("p0.pic=4");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      break;
    case 4:
      Serial2.print("p0.pic=6");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      break;
    case 5:
      Serial2.print("p0.pic=1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      break;
    case 6:
      Serial2.print("p0.pic=5");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      break;        
  }
}

STATE InformationDisplay::getState()
{
  return _state;
}

void InformationDisplay::setState(STATE state)
{
  _state = state;
}

#endif