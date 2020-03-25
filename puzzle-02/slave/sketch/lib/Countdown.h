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
    );
    void update();
    unsigned long getTime();
    void resetTime();
    STATE getState();
    unsigned long getmaxTimeCount();
    void display();
    void setState(STATE);
    void setmaxTimeCount(unsigned long);

  private:
    Adafruit_7segment * _matrix;
    unsigned long timeCounter;
    unsigned long maxTimeCount;
    STATE _state;
    struct Timer {
      unsigned long current = 0;
      unsigned long countPoint = 0;
      unsigned long interval = 1000;
    } Timer;
};

Countdown::Countdown(){
    maxTime = 600000;
    timeCounter  = maxTimeCount
}

void Countdown::set(Adafruit_7segment *matrix){
    _matrix = matrix;
}

unsigned long Countdown::getmaxTimeCount() 
{
  return maxTimeCount;
} 

void Countdown::setmaxTimeCount(unsigned long _maxTimeCount) 
{
  maxTimeCount = _maxTimeCount;
} 

STATE Countdown::getState() 
{
  return _state;
}
void Countdown::setState(STATE state) {
  _state = state;
}

unsigned long Countdown::getTime() {
  return timeCounter;
}

void Countdown::update()
{
    c.Timer.current = millis();
    switch (_state)
    {
      case ENABLE:
        if((c.Timer.current - c.Timer.countPoint) > c.Timer.interval){
          timeCounter--;
          c.Timer.countPoint = millis();
        }
        break;

      case DISABLE:
        c.Timer.countPoint = c.Timer.current;
        break;

      case PAUSE: 
        c.Timer.countPoint = c.Timer.current;
        break;

      case RESET: 
        timeCounter = maxTimeCount;
        c.Timer.countPoint = c.Timer.current;
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