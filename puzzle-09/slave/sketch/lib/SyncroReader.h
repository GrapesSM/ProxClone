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
    void disable();
    void enable();
    void update();
    void display();
    bool isDisabled();
    bool isSynchronized();
    void setSynchronized(bool solved);
    int getInputKey();
    bool isSwitchOn();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int *_lightPins;
    bool _disabled = true;
    int _pin;
    bool _synchronized = false;
    int _count;
    int _inputKey;
    const unsigned long _waitTimeMillis = 4000; // ms
    unsigned long lastRefreshTime;
};

SyncroReader::SyncroReader() {}

void SyncroReader::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], int pin)
{
  _strip = strip;
  _lightPins = lightPins;
  _count = 0;
  _pin = pin;
  _inputKey = 0;
}

bool SyncroReader::isSwitchOn()
{
  return digitalRead(_pin);
}

bool SyncroReader::isSynchronized(){
  return _synchronized;
}

void SyncroReader::setSynchronized(bool synchronized = true){
  _synchronized = synchronized;
  for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_SYNCRO_READER; i++) {
    _strip->SetPixelColor(_lightPins[i], RgbColor(0, 127, 0));
  }
}

int SyncroReader::getInputKey() {
  return _inputKey;
}

void SyncroReader::disable()
{
  _disabled = true;
  for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_SYNCRO_READER; i++) {
    _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
  }
}

void SyncroReader::enable()
{
  _disabled = false;
}

bool SyncroReader::isDisabled()
{
  return _disabled;
}

void SyncroReader::update()
{
  Serial.println(digitalRead(_pin));
  _strip->SetPixelColor(_lightPins[0], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[1], RgbColor(0, 0, 0));
  _strip->SetPixelColor(_lightPins[2], RgbColor(0, 0, 0));

  _count = 0;
  if(millis() - lastRefreshTime >= _waitTimeMillis / 4){
    _strip->SetPixelColor(_lightPins[0], RgbColor(127, 127, 127));
    _count = 3;
    if(millis() - lastRefreshTime >= _waitTimeMillis * 2 / 4){
      _strip->SetPixelColor(_lightPins[1], RgbColor(127, 127, 127));
      _count = 2;
      if(millis() - lastRefreshTime >= _waitTimeMillis * 3 / 4){
        _strip->SetPixelColor(_lightPins[2], RgbColor(127, 127, 127));
        _count = 1;
        if(millis() - lastRefreshTime >= _waitTimeMillis){
          lastRefreshTime = millis();
        }
      }
    }
  }
  if(isSwitchOn()){
    if(_inputKey == 0){
      _inputKey = _count;
    }
  }else{
    _inputKey = 0;
  }
}

void SyncroReader::display()
{
  _strip->Show();
}

#endif