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
  _count = 0;
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

  if (_state == SYNCRONIZED) {
    for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_SYNCRO_READER; i++) {
      _strip->SetPixelColor(_lightPins[i], RgbColor(0, 127, 0));
    }
    return;
  }

  if (_state == ENABLE ) {
    _timer.start = millis();
    _timer.lastRefreshTime = _timer.start;
    _state = COUNTING;
  }

  if (_state == COUNTING) {
    if (millis() - _timer.lastRefreshTime >= _timer.waitTimeMillis / 4) {
      _strip->SetPixelColor(_lightPins[0], RgbColor(127, 127, 127));
      _count = 3;
      if (millis() - _timer.lastRefreshTime >= _timer.waitTimeMillis * 2 / 4) {
        _strip->SetPixelColor(_lightPins[1], RgbColor(127, 127, 127));
        _count = 2;
        if (millis() - _timer.lastRefreshTime >= _timer.waitTimeMillis * 3 / 4) {
          _strip->SetPixelColor(_lightPins[2], RgbColor(127, 127, 127));
          _count = 1;
          if (millis() - _timer.lastRefreshTime >= _timer.waitTimeMillis) {
            for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_SYNCRO_READER; i++) {
              _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
            }
            _state = ENABLE; //DONE
            _count = 0;
            _timer.lastRefreshTime = millis();
          }
        }
      }
    }
    if (isSwitch(HIGH)) {
      if(_inputKey == 0) {
        _inputKey = _count;
      }
    }else{
      _inputKey = 0;
    }
  }
  
}

void SyncroReader::display()
{
  _strip->Show();
}

#endif