/*
  SyncroReader.h - Library for playing sounds and voices.
*/
#ifndef SyncroReader_h
#define SyncroReader_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class SyncroReader
{
  public:
    SyncroReader();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin);
    void update();
    void display();
    int getInputKey();
    void setState(STATE state);
    STATE getState();
    bool isSwitch(int position);
    void startTimer();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int *_lightPins;
    int _pin;
    int _count;
    int _inputKey;
    struct Timer {
      unsigned long start;
      unsigned long lastRefreshTime;
      const unsigned long waitTimeMillis = 4000; // ms
    } _timer;
    STATE _state;
};

SyncroReader::SyncroReader() {}

void SyncroReader::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin)
{
  _strip = strip;
  _lightPins = lightPins;
  _pin = pin;
  _count = -1;
  _inputKey = 0;
}

bool SyncroReader::isSwitch(int position)
{
  return digitalRead(_pin) == position;
}

void SyncroReader::setState(STATE state)
{
  _state = state;
}

STATE SyncroReader::getState() 
{
  return _state;
}

int SyncroReader::getInputKey() 
{
  return _inputKey;
}

void SyncroReader::update()
{
  if (_state == DISABLE) {
    for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_SYNCRO_READER; i++) {
      _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
    }
    return;
  }

  if (_state == SOLVED) {
    for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_SYNCRO_READER; i++) {
      _strip->SetPixelColor(_lightPins[i], RgbColor(0, 127, 0));
    }
    return;
  }

  if (_state == START_TIMER ) {
    _timer.start = millis();
    _timer.lastRefreshTime = _timer.start;
    _state = COUNTING;
  }

  if (_state == FLASH){
    if(isSwitch(HIGH)){
      _strip->SetPixelColor(_lightPins[0], RgbColor(127, 0, 0));
      _strip->SetPixelColor(_lightPins[1], RgbColor(127, 0, 0));
      _strip->SetPixelColor(_lightPins[2], RgbColor(127, 0, 0));
    }else{
      _state = DONE;
      _inputKey = -1;
    }
  }

  if (_state == COUNTING) {
    if(isSwitch(HIGH)){
      if(_inputKey == -1) {
        _inputKey = _count;
      }
      _strip->SetPixelColor(_lightPins[0], RgbColor(255, 255, 0));
      _strip->SetPixelColor(_lightPins[1], RgbColor(255, 255, 0));
      _strip->SetPixelColor(_lightPins[2], RgbColor(255, 255, 0));
      return;
    }else{
      _inputKey = -1;
    }
    if (millis() - _timer.lastRefreshTime >= _timer.waitTimeMillis / 5) {
      _strip->SetPixelColor(_lightPins[0], RgbColor(127, 127, 127));
      _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
      _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
      _count = 0;
    }
    if (millis() - _timer.lastRefreshTime >= _timer.waitTimeMillis * 2 / 5) {
      _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
      _strip->SetPixelColor(_lightPins[1], RgbColor(127, 127, 127));
      _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
      _count = 1;
    }
    if (millis() - _timer.lastRefreshTime >= _timer.waitTimeMillis * 3 / 5) {
      _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
      _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
      _strip->SetPixelColor(_lightPins[2], RgbColor(127, 127, 127));
      _count = 2;
    }
    if (millis() - _timer.lastRefreshTime >= _timer.waitTimeMillis * 4 / 5) {
      _strip->SetPixelColor(_lightPins[0], RgbColor(127, 127, 127));
      _strip->SetPixelColor(_lightPins[1], RgbColor(127, 127, 127));
      _strip->SetPixelColor(_lightPins[2], RgbColor(127, 127, 127));
      _count = 3;
    }
    if (millis() - _timer.lastRefreshTime >= _timer.waitTimeMillis) {
      for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_SYNCRO_READER; i++) {
        _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
      }
      _state = DONE;
      _count = 4;
      _timer.lastRefreshTime = millis();
    }
  }
}

void SyncroReader::display()
{
  _strip->Show();
}

#endif