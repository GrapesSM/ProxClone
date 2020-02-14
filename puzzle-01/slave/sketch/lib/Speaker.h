/*
  Speaker.h - Library for playing sounds and voices.
*/
#ifndef Speaker_h
#define Speaker_h

#include <Arduino.h>
#include "Queue.h"

class Speaker
{
  public:
    Speaker();
    void set(
      int pin, 
      int enablePin, 
      unsigned int rate, 
      unsigned char** listOfSounds, 
      unsigned int* listOfLengthOfSounds);
    void init();
    void enable();
    void disable();
    void isDisabled();
    STATE getState();
    void play();
    void play(int number);
    void speak(int frequency = PWM_FREQUENCY, int dutycycle = PWM_DUTYCYCLE);
    void addToPlay(int number);

  private:
    int _pin;
    int _enablePin;
    bool _disabled;
    int _counter;
    unsigned int _rate;
    unsigned char** _listOfSounds;
    unsigned int* _listOfLengthOfSounds;
    int _numberOfSounds;
    int _frequency;
    int _dutycycle;
    STATE _state;
    DataQueue<unsigned int> _queue;
};

Speaker::Speaker() 
{
  _disabled = true;
  _counter = 0;
  _numberOfSounds = 0;
}

void Speaker::set(
  int pin, 
  int enablePin, 
  unsigned int rate,
  unsigned char** listOfSounds,
  unsigned int* listOfLengthOfSounds)
{
  _pin = pin;
  _enablePin = enablePin;
  _rate = rate;
  _listOfSounds = listOfSounds;
  _listOfLengthOfSounds = listOfLengthOfSounds;
  _numberOfSounds = sizeof(listOfSounds)/sizeof(unsigned char*);
}

void Speaker::init() {
  Serial.println("Speaker: Init");
}

STATE Speaker::getState()
{
  return _state;
}

void Speaker::enable()
{
  _state = ON;
  _disabled = false;
}

void Speaker::disable()
{
  _state = OFF;
  _disabled = true;
}

void Speaker::play(int number)
{
  _state = PLAYING;
  for (int i = 0; i < _listOfLengthOfSounds[number]; i++) {
    dacWrite(_pin, _listOfSounds[number][i]);
    delayMicroseconds(_rate);
  }
  _state = STANDBY;
}

void Speaker::speak(int frequency, int dutycycle)
{
  if (_frequency != frequency) {
    _frequency = frequency;
    ledcWriteTone(PWM_CHANNEL, frequency);
  }

  if (_dutycycle != dutycycle) {
    _dutycycle = dutycycle;
    ledcWrite(PWM_CHANNEL, dutycycle);
  }
  delayMicroseconds(_rate);
}

void Speaker::addToPlay(int number)
{
  _queue.enqueue(number);
}

void Speaker::play()
{
  if (_queue.isEmpty()) {
    return;
  }
  Serial.println(_queue.item_count());
  play(_queue.dequeue());
}

#endif