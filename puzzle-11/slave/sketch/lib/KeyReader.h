/*
  KeyReader.h - Library for ________.
*/
#ifndef KeyReader_h
#define KeyReader_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class KeyReader
{
  public:
    KeyReader();
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

KeyReader::KeyReader() {}

void KeyReader::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[])
{
  _strip = strip;
  _lightPins = lightPins;
}

void KeyReader::disable()
{
  _disabled = true;
  // TO-DO:
}

void KeyReader::enable()
{
  _disabled = false;
  // TO-DO:
}

bool KeyReader::isDisabled()
{
  return _disabled;
}

void KeyReader::update()
{
  // TO-DO:
}

void KeyReader::display()
{
  _strip->Show();
}

#endif