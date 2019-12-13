/*
  CodeReader.h - Library for playing sounds and voices.
*/
#ifndef CodeReader_h
#define CodeReader_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class CodeReader
{
  public:
    CodeReader();
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

CodeReader::CodeReader() {}

void CodeReader::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void CodeReader::disable()
{
  _disabled = true;
  // TO-DO:
}

void CodeReader::enable()
{
  _disabled = false;
  // TO-DO:
}

bool CodeReader::isDisabled()
{
  return _disabled;
}

void CodeReader::update()
{
  // TO-DO:
}

void CodeReader::display()
{
  // TO-DO:
}

#endif