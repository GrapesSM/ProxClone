/*
  CodeReader.h - Library for ________.
*/
#ifndef CodeReader_h
#define CodeReader_h

#include <Arduino.h>
#include <Adafruit_MCP23017.h>
#include "SevenSegment.h"
#include "DebounceSwitch.h"

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

    String getTransmittedKey();
    MODE readMode();
    STATE getState();
    void setState(STATE state);
  private:
    SevenSegment *_matrix;
    Adafruit_MCP23017 *_mcp1;
    int *_buttonPins1;
    Adafruit_MCP23017 *_mcp2;
    int *_buttonPins2;
    char *_buttonLabels;
    bool _disabled;
    int _counter;
    bool _entered;
    String _key;
    bool _transmitted;
    String _transmittedKey;
    DebounceSwitch _modeSwitch;
    DebounceSwitch _transmitSwitch;
    int _reading[10];
    int _switchState[10];
    int _lastSwitchState[10];
    unsigned long _lastDebounceTime[10];
    unsigned long _debounceDelay = 50;
    STATE _state;
};

CodeReader::CodeReader() {
  _entered = false;
  _counter = 0;
  _transmitted = false;
  _key = "";
  _transmittedKey = "";
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
  _modeSwitch.set(modePin);
  _transmitSwitch.set(transmitPin);  
}

String CodeReader::getTransmittedKey() {
  return _transmittedKey;
}

char CodeReader::readInput()
{
  String _buttons = String("0000000000");
  char val;
  for (int i = 0; i < NUMBER_OF_BUTTONS_1; i++) {
    _reading[i] = _mcp1->digitalRead(_buttonPins1[i]);
    if (_reading[i] != _lastSwitchState[i]) {
      _lastDebounceTime[i] = millis();
    }

    if ((millis() - _lastDebounceTime[i]) > _debounceDelay) {
      // if (_reading[i] != _switchState[i]) {
        _switchState[i] = _reading[i];
        // Serial.print(_switchState[i]);
      // }
    }

    _lastSwitchState[i] = _reading[i];
    _buttons.setCharAt(i, _switchState[i] ? '1' : '0');
  }

  for (int i = NUMBER_OF_BUTTONS_1, n = NUMBER_OF_BUTTONS_1 + NUMBER_OF_BUTTONS_2; i < n; i++) {
    _reading[i] = _mcp2->digitalRead(_buttonPins2[i]);
    if (_reading[i] != _lastSwitchState[i]) {
      _lastDebounceTime[i] = millis();
    }

    if ((millis() - _lastDebounceTime[i]) > _debounceDelay) {
      // if (_reading[i] != _switchState[i]) {
        _switchState[i] = _reading[i];
        // Serial.print(_switchState[i]);
      // }
    }

    _lastSwitchState[i] = _reading[i];
    _buttons.setCharAt(i, _switchState[i] ? '1' : '0');

  }
  // Serial.println();

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
  if (_modeSwitch.isSwitch(HIGH)) {
    return CLEAR_MODE;
  } else {
    return INPUT_MODE;
  }
}

void CodeReader::update()
{
  _modeSwitch.readSwitch();
  _transmitSwitch.readSwitch();
  MODE mode;
  char input; 
  switch (_state)
  {

    case DISABLE:
      // Serial.println("DISABLE");
      break;

    case ENABLE:
    default:
      input = readInput();
      mode = readMode();
      switch(mode) {
        case INPUT_MODE: 
          // Update number -----------------
          if (input != 'n' && _entered == false && _counter < _matrix->getNumberOfDigits()) {
            _counter++;
            _key += String(input);
            _entered = true;
          } 

          if (_entered == true && input == 'n') {
            _entered = false;
          }

          if (input == 'n') {
            _entered = false;
          }
          // ------------------------------
          break;
        case CLEAR_MODE:
          _key = "";
          _entered = false;
          _counter = 0;
          _transmittedKey = "";
          break;
      } 

      if (_transmitSwitch.isSwitch(HIGH) && _transmitted == false) {
        _transmitted = true;
        _transmittedKey = _key;
      }
      if (_transmitSwitch.isSwitch(LOW) && _transmitted == true) {
        _transmitted = false;
      }
      break;
  }
}

void CodeReader::display()
{
  switch (_state)
  {
    case DISABLE:
      _matrix->clear();
      _matrix->writeDisplay();
      break;

    case ENABLE:
    default:
      _matrix->clear();
      if (_key.length() > 0) _matrix->printString(_key);
      _matrix->writeDisplay();
      break;
  }
}

STATE CodeReader::getState()
{
  return _state;
}

void CodeReader::setState(STATE state)
{
  _state = state;
}

#endif
