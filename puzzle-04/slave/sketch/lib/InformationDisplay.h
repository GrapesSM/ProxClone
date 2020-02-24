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
    void enable();
    void frameShowSeries();
    void setCurrentSeries(int);
    STATE getState();
    void setState(STATE state);
  private:
    ESP32Encoder *_encoder;
    int _val;
    int _min;
    int _max;
    int _disabled = true;
    int _currentSeries;
    STATE _state;
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
  _currentSeries = 0;
}
void InformationDisplay::setCurrentSeries(int current){
  _currentSeries = current;
}
void InformationDisplay::frameShowSeries(){
  _val = _encoder->getCount();
  if (_val < _min) {
    _val = _max;
    _encoder->setCount(_val);
  }
  if(_val > _max){
    _val = _min; 
    _encoder->setCount(_val);
  }
  switch(_currentSeries){
    case 0:
      Serial2.print("p0.pic=0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      break;

    case 1:
      if((_val/10)%2 == 1){
        Serial2.print("p0.pic=1");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        break;
      }else{
        Serial2.print("p0.pic=2");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        break;
      }
    case 2:
      if((_val/10)%3 == 2){
        Serial2.print("p0.pic=3");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        break;
      }else if((_val/10)%3 == 1){
        Serial2.print("p0.pic=4");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        break;
      }else{
        Serial2.print("p0.pic=5");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        break;        
      }
  }
}

void InformationDisplay::update() {
  if(_state == DISABLE) return;
  if(_state == ENABLE)
  {
    frameShowSeries();
  }
}

void InformationDisplay::enable() {
  _disabled = false;
  _state = ON;
  _encoder->resumeCount();
}

void InformationDisplay::display() {
  // Serial.println(_val);
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