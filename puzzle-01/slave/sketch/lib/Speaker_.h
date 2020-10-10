/*
  Speaker.h - Library for playing sounds and voices.
*/
#ifndef Speaker_h
#define Speaker_h

#include <Arduino.h>
#include "Queue.h"
#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"

class Speaker
{
  public:
    Speaker();
    void init();
    void set(int pin, int enablePin, AudioFileSourceSPIFFS* files[]);
    void setState(STATE state);
    STATE getState();
    void play();
    void addToPlay(int number);
    void update();
    void setNumber(int number);
    int getNumber();
  private:
    int _pin;
    int _enablePin;
    int _channel;
    int _counter;
    int _number;
    int _index;
    bool _flag = false;
    AudioFileSourceSPIFFS** _files;
    AudioFileSourceSPIFFS* _file;
    AudioGeneratorMP3 *_mp3;
    AudioOutputI2SNoDAC *_out;
    AudioFileSourceID3 *_id3;
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
  _number = -1;
  _index = 0;
}

void Speaker::set(
  int pin, 
  int enablePin,
  AudioFileSourceSPIFFS* files[])
{
  _pin = pin;
  _enablePin = enablePin;
  _files = files;
}

void Speaker::setState(STATE state)
{
  _state = state;
}

STATE Speaker::getState()
{
  return _state;
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
  // switch (_state)
  // {
  //   case ENABLE:
  //     if (digitalRead(PIN_AMPLIFIER) == LOW) {
  //       digitalWrite(PIN_AMPLIFIER, HIGH);
  //       vTaskDelay(5);
  //     }
  //     break;
    
  //   case DISABLE:
  //     if (digitalRead(PIN_AMPLIFIER) == HIGH) {
  //       digitalWrite(PIN_AMPLIFIER, LOW);
  //       vTaskDelay(5);
  //     }
  //     break;
  // }
}

void Speaker::play() 
{
  unsigned int SoundFile = SOUND_POWER_UP; //_queue.dequeue();
  if (_flag == false) {
    _file = new AudioFileSourceSPIFFS("/PowerOn.mp3");
    _id3 = new AudioFileSourceID3(_file);
    _out = new AudioOutputI2SNoDAC();
    _mp3 = new AudioGeneratorMP3();
    _mp3->begin(_id3, _out);
    _flag = true;
  }

  if (_mp3->isRunning()) {
    if (!_mp3->loop()) _mp3->stop();
  } else {
    vTaskDelay(200);
    _flag = false;
  }
  
  // if (_queue.isEmpty()) {  
  //   _flag = true;
  //   return;
  // }

  // if (_flag) {
  //   _flag = false;
  // }
  
}

#endif