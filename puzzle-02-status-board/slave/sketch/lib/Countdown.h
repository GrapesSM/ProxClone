#ifndef Countdown_h
#define Countdown_h

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

class Countdown
{
  public:
    Countdown();
    void init();
    void set(
      Adafruit_7segment *_matrix
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
    } timer;
};

Countdown::Countdown()
{
  init();
}

void Countdown::init()
{
  maxTimeCount = 3600;
  timeCounter  = maxTimeCount;
}

void Countdown::set(Adafruit_7segment *matrix){
  _matrix = matrix;
}

unsigned long Countdown::getmaxTimeCount() 
{
  return maxTimeCount;
}

void Countdown::resetTime()
{
  timeCounter = maxTimeCount;
  timer.countPoint = timer.current;
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
  timer.current = millis();
  switch (_state)
  {
    case ENABLE:
      if((timer.current - timer.countPoint) > timer.interval){
        timeCounter--;
        timer.countPoint = millis();
      }
      break;

    case DISABLE:
      timer.countPoint = timer.current;
      break;

    case PAUSE: 
      timer.countPoint = timer.current;
      break;

    case RESET: 
      timeCounter = maxTimeCount;
      timer.countPoint = timer.current;
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
      _matrix->clear();
      _matrix->print(((timeCounter / 60) * 100 + timeCounter % 60) % 10000);
      _matrix->writeDisplay();
      break;
  }
}
#endif