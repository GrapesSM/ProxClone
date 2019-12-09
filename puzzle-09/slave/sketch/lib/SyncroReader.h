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
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int *_lightPins;
};

SyncroReader::SyncroReader() {}

void SyncroReader::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

#endif