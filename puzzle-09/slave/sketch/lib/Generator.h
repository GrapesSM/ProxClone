/*
  Generator.h - Library for playing sounds and voices.
*/
#ifndef Generator_h
#define Generator_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class Generator
{
  public:
    Generator();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], Adafruit_MCP23017 *mcp, int switchPins[], int labels[]);
    void update();
    void disable();
    void enable();
    bool isDisabled();
    bool isAllSwitchesOff();
    void readSwitches();
    int getInputKey();
    bool isSolved();
    void setSolved(bool solved);
  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *_strip;
    int *_lightPins;
    Adafruit_MCP23017 *_mcp;
    bool _disabled = true;
    bool _solved;
    int *_switchPins;
    int _input[7];
    int _order[10];
    int _count;
    bool _reset;
    int *_labels;
};

Generator::Generator(){}

void Generator::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPins[], Adafruit_MCP23017 *mcp, int switchPins[], int labels[])
{
  _strip = strip;
  _lightPins = lightPins;
  _mcp = mcp;
  _switchPins = switchPins;
  _labels = labels;
  _count = 0;
  _reset = false;
  _solved = false;
}

void Generator::update()
{
  readSwitches();
  getInputKey();
  
  if (! _reset) {
    for (int i = 0; i < NUMBER_OF_SWITCHES_2; i++) {
      if (_input[i] == HIGH && _order[_labels[i]] == 0) {
        _order[_labels[i]] = ++_count;
        _strip->SetPixelColor(_lightPins[i], RgbColor(127, 127, 127));
      }

      if (_input[i] == LOW && _order[_labels[i]] != 0) {
        _reset = true;
      }     
    }
  } 

  if (_reset) {
    for (int i = 0; i < NUMBER_OF_SWITCHES_2; i++) {
      if (_input[i] == HIGH) {
        _strip->SetPixelColor(_lightPins[i], RgbColor(127, 0, 0));
      } else {
        _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
      }
    }
  }

  if (isAllSwitchesOff()) {
    _reset = false;
    for (int i = 0; i < NUMBER_OF_SWITCHES_2; i++) {
      _order[_labels[i]] = 0;
    }
    _count = 0;
  }
}

bool Generator::isSolved() 
{
  return _solved;
} 

void Generator::setSolved(bool solved = true) 
{
  _solved = solved;
  for (int i = 0; i < NUMBER_OF_SWITCHES_2; i++) {
    _strip->SetPixelColor(_lightPins[i], RgbColor(0, 127, 0));
  }
} 

bool Generator::isAllSwitchesOff() 
{
  bool check = true;
  for (int i = 0; i < NUMBER_OF_SWITCHES_2; i++) {
    if (_input[i] == HIGH) {
      check = false;
    }
  }
  return check;
}

void Generator::readSwitches()
{
  _input[0] = LOW;
  for (int i = 0; i < NUMBER_OF_SWITCHES_2; i++) {
    _input[i] = _mcp->digitalRead(_switchPins[i]);
  }
}

int Generator::getInputKey() {
  int input = 0;
  int max = 0;
  for (int i = 0; i < NUMBER_OF_SWITCHES_2; i++) {
    if(max < _order[i]){max = _order[i];}
  }
  for (int i = 0; i < NUMBER_OF_SWITCHES_1; i++) {
    if(_order[_labels[i]] != 0){
      input +=  pow(10,(max-_order[_labels[i]])) * _labels[i]  ;
    }
  }
  return input;
}

void Generator::disable() 
{
  _disabled = true;
  for (int i = 0; i < NUMBER_OF_SWITCHES_2; i++) {
    _strip->SetPixelColor(_lightPins[i], RgbColor(0, 0, 0));
  }
}

void Generator::enable() 
{
  _disabled = false;
}

bool Generator::isDisabled()
{
  return _disabled;
}


#endif