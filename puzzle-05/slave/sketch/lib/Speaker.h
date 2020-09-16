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
    void init();
    void set(
      int pin, 
      int enablePin, 
      unsigned int rate, 
      unsigned char** listOfSounds, 
      unsigned int* listOfLengthOfSounds,
      int channel);
    void setState(STATE state);
    STATE getState();
    void play();
    void play(int number);
    void speak(int frequency = PWM_SPEAKER_FREQUENCY, int dutycycle = PWM_SPEAKER_DUTYCYCLE);
    void addToPlay(int number);
    void update();
    void setNumber(int number);
    int getNumber();
  private:
    int _pin;
    int _enablePin;
    int _channel;
    int _counter;
    unsigned int _rate;
    unsigned char** _listOfSounds;
    unsigned int* _listOfLengthOfSounds;
    int _numberOfSounds;
    int _frequency;
    int _dutycycle;
    int _number;
    int _index;
    STATE _state;
    DataQueue<unsigned int> _queue;
};

Speaker::Speaker() 
{
  init();
}

void Speaker::init()
{
  _counter = 0;
  _numberOfSounds = 0;
  _number = -1;
  _index = 0;
}

void Speaker::set(
  int pin, 
  int enablePin, 
  unsigned int rate,
  unsigned char** listOfSounds,
  unsigned int* listOfLengthOfSounds,
  int channel)
{
  _pin = pin;
  _enablePin = enablePin;
  _channel = channel;
  _rate = rate;
  _listOfSounds = listOfSounds;
  _listOfLengthOfSounds = listOfLengthOfSounds;
  _numberOfSounds = sizeof(listOfSounds)/sizeof(unsigned char*);
}

void Speaker::setState(STATE state)
{
  _state = state;
}

STATE Speaker::getState()
{
  return _state;
}

void Speaker::play(int number)
{
  for (int i = 0; i < _listOfLengthOfSounds[number]; i++) {
    dacWrite(_pin, _listOfSounds[number][i]);
    delayMicroseconds(_rate);
  }
}

void Speaker::speak(int frequency, int dutycycle)
{
  if (_frequency != frequency) {
    _frequency = frequency;
    ledcWriteTone(_channel, frequency);
  }

  if (_dutycycle != dutycycle) {
    _dutycycle = dutycycle;
    ledcWrite(_channel, dutycycle);
  }

  if (_frequency && dutycycle) {
    delayMicroseconds(_rate);
  }
}

void Speaker::addToPlay(int number)
{
  _number = number;
  _queue.enqueue(number);
}

void Speaker::setNumber(int number) 
{
  _number = number;
}

int Speaker::getNumber()
{
  return _number;
}

void Speaker::update()
{
  switch (_state)
  {
    case ENABLE:
      if (digitalRead(PIN_AMPLIFIER) == LOW) {
        digitalWrite(PIN_AMPLIFIER, HIGH);
        delay(5);
      }
      break;
    
    case DISABLE:
      if (digitalRead(PIN_AMPLIFIER) == HIGH) {
        digitalWrite(PIN_AMPLIFIER, LOW);
        delay(5);
      }
      break;
  }
}

void Speaker::play() 
{
  if (_queue.isEmpty()) {      
    return;
  }
  _state = ENABLE;
  update();
  play(_queue.dequeue());    
  _state = DISABLE;
  update();
}

#endif