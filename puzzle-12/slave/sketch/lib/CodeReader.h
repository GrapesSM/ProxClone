#ifndef CodeReader_h
#define CodeReader_h

#include <Arduino.h>
#include "SparkFun_Qwiic_Keypad_Arduino_Library.h"
#include "DebounceSwitch.h"

class CodeReader
{
  public:
    CodeReader();
    void set(
      KEYPAD *keypad,
      int pin
    );
    void update();
    void clear();
    String getInputKey();
    STATE getState();
    void setState(STATE);
  private:
    String _input;
    DebounceSwitch _dSwitch;
    KEYPAD *_keypad;
    STATE _state;
};

CodeReader::CodeReader(){
  _input = "";
}

void CodeReader::set(KEYPAD *keypad, int pin){
  _keypad = keypad;
  _dSwitch.set(pin);
}

STATE CodeReader::getState() 
{
  return _state;
}
void CodeReader::setState(STATE state) {
  _state = state;
}

String CodeReader::getInputKey() {
  return _input;
}

void CodeReader::update()
{
  if (_state == DISABLE) {
    return;
  }

  _dSwitch.readSwitch();
  
  _keypad->updateFIFO();  // necessary for keypad to pull button from stack to readable register
  char key = _keypad->getButton();

  if (key == -1)
  {
    // No keypad detected
    return;
  }
  
  if (key != 0)
  {
    _state = KEY_ENTERED;
    if(_input.length() <= 10){
      _input += key;
    }
  }
  //Do something else. Don't call your Keypad a ton otherwise you'll tie up the I2C bus
  delay(25); //25 is good, more is better

  if (key == 0) {
    _state = DONE;
  }

  if (_dSwitch.isSwitch(HIGH)) {
    _dSwitch.readSwitch();
    if (_dSwitch.isSwitch(LOW)) {
      _state = TRANSMITTED;
    }
  }
}

void CodeReader::clear() {
  _input = "";
}

#endif