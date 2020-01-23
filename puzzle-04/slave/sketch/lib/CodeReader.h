/*
  CodeReader.h - Library for ________.
*/
#ifndef CodeReader_h
#define CodeReader_h

#include <Arduino.h>
#include <Adafruit_MCP23017.h>
#include "SevenSegment.h"

enum MODE {
  INPUT_MODE = 1,
  CLEAR_MODE = 2
};

class CodeReader
{
  public:
    CodeReader();
    void set(
      SevenSegment *matrix, 
      Adafruit_MCP23017 *mcp1,
      int buttonPins1[],
      Adafruit_MCP23017 *mcp2,
      int buttonPins2[],
      char buttonLabels[],
      int modePin,
      int transmitPin
      );
    void disable();
    void enable();
    void update();
    void display();
    bool isDisabled();
    char readInput();
    String getInputKey();
    MODE readMode();
    bool isSolved();
    void setSolved(bool);
  private:
    SevenSegment *_matrix;
    Adafruit_MCP23017 *_mcp1;
    int *_buttonPins1;
    Adafruit_MCP23017 *_mcp2;
    int *_buttonPins2;
    char *_buttonLabels;
    int _modePin;
    int _transmitPin;
    bool _disabled;
    char _input;
    String _number;
    bool _entered;
    int _counter;
    bool _solved;
    bool _transmittingMode = false;
    String _transmitInput;
};

CodeReader::CodeReader() {
  _disabled = true;
  _input = 'n';
  _number = "";
  _entered = false;
  _counter = 0;
  _solved = false;
}

void CodeReader::set(
      SevenSegment *matrix, 
      Adafruit_MCP23017 *mcp1,
      int buttonPins1[],
      Adafruit_MCP23017 *mcp2,
      int buttonPins2[],
      char buttonLabels[],
      int modePin,
      int transmitPin)
{
  _matrix = matrix;
  _mcp1 = mcp1;
  _buttonPins1 = buttonPins1;
  _mcp2 = mcp2;
  _buttonPins2 = buttonPins2;
  _buttonLabels = buttonLabels;
  _modePin = modePin;
  _transmitPin = transmitPin;
  _transmitInput = "";
}

bool CodeReader::isSolved() {
  return _solved;
}

void CodeReader::setSolved(bool solved = true) {
  _solved = solved;
  _number = "5";
}


String CodeReader::getInputKey() {
  return _number;
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

char CodeReader::readInput()
{
  String _buttons = String("0000000000");

  for (int i = 0; i < NUMBER_OF_BUTTONS_1; i++) {
    _buttons.setCharAt(i, _mcp1->digitalRead(_buttonPins1[i]) ? '1' : '0');
  }

  for (int i = NUMBER_OF_BUTTONS_1, n = NUMBER_OF_BUTTONS_1 + NUMBER_OF_BUTTONS_2; i < n; i++) {
    _buttons.setCharAt(i, _mcp2->digitalRead(_buttonPins2[i]) ? '1' : '0');
  }

  switch (strtol( _buttons.c_str(), NULL, 2 ))
  {
    case 512:
      return _buttonLabels[0];    
    case 256:
      return _buttonLabels[1];
    case 128:
      return _buttonLabels[2];
    case 64:
      return _buttonLabels[3];
    case 32:
      return _buttonLabels[4];
    case 16:
      return _buttonLabels[5];
    case 8:
      return _buttonLabels[6];
    case 4:
      return _buttonLabels[7];
    case 2:
      return _buttonLabels[8];
    case 1:
      return _buttonLabels[9];    
    
    default:
      return 'n';
  }
}

MODE CodeReader::readMode()
{
  if (digitalRead(_modePin) == HIGH) {
    return INPUT_MODE;
  } else {
    return CLEAR_MODE;
  }
}

void CodeReader::update()
{
  if(_transmittingMode == false){
    char val = readInput();
    MODE mode = readMode();
    switch(mode) {
      case INPUT_MODE: 
        // Update number -----------------
        if (val != 'n') {
          _entered = true;
          _input = val;
        }
        if (_entered && val == 'n' && _counter < 5) {
          _counter++;
          _number += _input;
          _entered = false;
        }
        // ------------------------------
        break;
      case CLEAR_MODE:
        _input = 'n';
        _number = "";
        _entered = false;
        _counter = 0;
        break;
    } 
    if(digitalRead(_transmitPin) == HIGH){
      _transmitInput = _input;
      _transmittingMode = true;
    }
  }else{
    _number = "00000";
  }
}

void CodeReader::display()
{
  _matrix->clear();
  if (_counter > 0) _matrix->printString(_number);
  _matrix->writeDisplay();
}


#endif
