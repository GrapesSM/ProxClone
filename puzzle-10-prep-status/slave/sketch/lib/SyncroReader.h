/*
  SyncroReader.h - Library for playing sounds and voices.
*/
#ifndef SyncroReader_h
#define SyncroReader_h

#include <Arduino.h>
#include "DebounceSwitch.h"

class SyncroReader
{
  public:
    SyncroReader();
    void init();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin);
    void update();
    void display();
    int getInputKey();
    void setState(STATE state);
    bool getSyncroKeyState();
    int getSyncroCountdownState();
    STATE getState();
    void startTimer();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int *_lightPins;
    DebounceSwitch _dSyncroKey;
    int _count;
    int _inputKey;
    struct Timer {
      unsigned long start;
      unsigned long current;
      unsigned long lastRefreshTime;
      const unsigned long waitTimeMillis = 900; // ms
    } _timer;
    STATE _state;
};

SyncroReader::SyncroReader() 
{
  init();
}

void SyncroReader::init()
{
  _count = -1;
  _inputKey = 10;
}

void SyncroReader::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin)
{
  _strip = strip;
  _lightPins = lightPins;
  _dSyncroKey.set(pin);
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

bool SyncroReader::getSyncroKeyState()
{
  return _dSyncroKey.getState();
}

int SyncroReader::getSyncroCountdownState()
{
  if (_count > 0 && _count < 5)
    return _count;
  return 0;
}

void SyncroReader::update()
{
  if (_state == DISABLE) {
    for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_SYNCRO_READER; i++) {
      _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
    }
    return;
  }

  if (_state == SYNCRONIZED) {
    for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_SYNCRO_READER; i++) {
      _strip->SetPixelColor(_lightPins[i], RgbColor(0, 127, 0));
    }
    return;
  }

  _dSyncroKey.readSwitch();

  while (_state == COUNTING) {
    _dSyncroKey.readSwitch();
    if (_dSyncroKey.isSwitch(HIGH)) {
      if (_inputKey == 10) {
        _inputKey = _count;
      }

      if (_count == 4) {
        _strip->SetPixelColor(_lightPins[0], RgbColor(255, 255, 0));
        _strip->SetPixelColor(_lightPins[1], RgbColor(255, 255, 0));
        _strip->SetPixelColor(_lightPins[2], RgbColor(255, 255, 0));
      } else {
        _strip->SetPixelColor(_lightPins[0], RgbColor(255, 0, 0));
        _strip->SetPixelColor(_lightPins[1], RgbColor(255, 0, 0));
        _strip->SetPixelColor(_lightPins[2], RgbColor(255, 0, 0));
      }

      return;
    } else {
      _inputKey = 10;
    }

    if (_count == -1 || (millis() - _timer.lastRefreshTime) >= _timer.waitTimeMillis) {
      _strip->Show();
      _timer.lastRefreshTime = millis();
      if (_count == -1) _count = 0;
      _count = _count < 5 ? _count + 1 : 0;
      _state = COUNTING;
    }
    
    switch (_count)
    {
      case 1:
        _strip->SetPixelColor(_lightPins[0], RgbColor(127, 127, 127));
        _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
        _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
        break;
      case 2:
        _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
        _strip->SetPixelColor(_lightPins[1], RgbColor(127, 127, 127));
        _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
        break;
      case 3:
        _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
        _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
        _strip->SetPixelColor(_lightPins[2], RgbColor(127, 127, 127));
        break;
      case 4:
        _strip->SetPixelColor(_lightPins[0], RgbColor(127, 127, 127));
        _strip->SetPixelColor(_lightPins[1], RgbColor(127, 127, 127));
        _strip->SetPixelColor(_lightPins[2], RgbColor(127, 127, 127));
        break;
      case 5:
        _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
        _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
        _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));
        break;
      case 0:
        _count = -1;
        _state = DONE;
        break;
      default:
        break;
    }
  }

  if (_state == FLASH){
    if (_dSyncroKey.isSwitch(HIGH)) {
      _strip->SetPixelColor(_lightPins[0], RgbColor(127, 0, 0));
      _strip->SetPixelColor(_lightPins[1], RgbColor(127, 0, 0));
      _strip->SetPixelColor(_lightPins[2], RgbColor(127, 0, 0));
    } else {
      _state = DONE;
      _inputKey = -1;
    }
  }
}

void SyncroReader::startTimer() {
  _timer.start = millis();
  _timer.lastRefreshTime = _timer.start;
  _state = COUNTING;
}

void SyncroReader::display()
{
  // _strip->Show();
}

#endif