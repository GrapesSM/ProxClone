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
    void setState(STATE state);
    STATE getState();
    void play();
    void play(int number);
    void speak(int frequency = PWM_SPEAKER_FREQUENCY, int dutycycle = PWM_SPEAKER_DUTYCYCLE);
    void addToPlay(int number);

  private:
    int _pin;
    int _enablePin;
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
    ledcWriteTone(PWM_SPEAKER_CHANNEL, frequency);
  }

  if (_dutycycle != dutycycle) {
    _dutycycle = dutycycle;
    ledcWrite(PWM_SPEAKER_CHANNEL, dutycycle);
  }

  if (_frequency && dutycycle) {
    delayMicroseconds(_rate);
  }
}

void Speaker::addToPlay(int number)
{
  _queue.enqueue(number);
}

void Speaker::play()
{
  switch (_state)
  {
    case ON:
      if (_queue.isEmpty()) {
        speak(0, 0);
        return;
      }
      play(_queue.dequeue());    
      break;
    
    case OFF:
      speak(PWM_SPEAKER_FREQUENCY, 0);
      break;
    
    case ALARM:
      unsigned long sec = millis()/1000; 
      if (sec % 3 == 0) {
        speak(1000);
      } else {
        speak(PWM_SPEAKER_FREQUENCY, 0);
      }
      break;
  }
}

#endif