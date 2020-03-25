#ifndef Countdown_h
#define Countdown_h

#include <Arduino.h>
#include "SevenSegment.h"

class Countdown
{
  public:
    Countdown();
    void set(
      Adafruit_7segment *_matrix,
      unsigned long time,
      unsigned long lastStoppedTime,
      unsigned long maxTime
    );
    void update();
    unsigned long getTime();
    void resetTime();
    STATE getState();
    void display();
    void setState(STATE);
  private:
    Adafruit_7segment * _matrix;
    unsigned long time;
    unsigned long lastStoppedTime;
    STATE _state;
};

Countdown::Countdown(){
    time = 0;
    lastStoppedTime = 0;
    maxTime = 0;
}

void Countdown::set(Adafruit_7segment *matrix){
    _matrix = matrix;
    time = 0
    lastStoppedTime = 0;
    maxTime = 600000;
}

STATE Countdown::getState() 
{
  return _state;
}
void Countdown::setState(STATE state) {
  _state = state;
}

unsigned long Countdown::getTime() {
  return time;
}

void Countdown::update()
{
    switch (_state)
    {
        case ENABLE:
            time = maxTime - (millis() -lastStoppedTime);
            break;

        case DISABLE:
            lastStoppedTime = millis();
            break;

        case PAUSE: 
            lastStoppedTime = millis();
            break;

        case RESET: 
            lastStoppedTime = 0;
            time = 0;
            break;
    }
}

void Countdown::display()
{
  switch (_state)
  {
    case DISABLE:
      _matrix->clear();
      _matrix->writeDisplay();
      break;

    case ENABLE:
    default:
       Serial.println(_key);
      _matrix->clear();
      _matrix->printString(_key);
      _matrix->writeDisplay();
      break;
  }
}
#endif