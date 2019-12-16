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
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[]);
    void disable();
    void enable();
    void update();
    void display();
    bool isDisabled();
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int *_lightPins;
    bool _disabled = true;
};

SyncroReader::SyncroReader() {}

void SyncroReader::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
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
  for (int i = 0; i < NUMBER_OF_LIGHTS_FOR_SYNCRO_READER; i++) {
    _strip->SetPixelColor(_lightPins[i], RgbColor(127, 127, 127));
  }
}

bool SyncroReader::isDisabled()
{
  return _disabled;
}

void SyncroReader::update()
{
  // TO-DO: Reader Input
}

void SyncroReader::display()
{
  _strip->Show();
}

#endif