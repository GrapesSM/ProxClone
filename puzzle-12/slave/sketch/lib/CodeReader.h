#ifndef CodeReader_h
#define CodeReader_h

#include <Arduino.h>
#include "SparkFun_Qwiic_Keypad_Arduino_Library.h"

class CodeReader
{
  public:
    CodeReader();
    void set(
      KEYPAD *keypad,
      int pin
    );
    void update();
    String getInputKey();
    STATE getState();
    void setState(STATE);
    bool isButton(int);
  private:
    String _input;
    int _pin;
    KEYPAD *_keypad;
    STATE _state;
};

CodeReader::CodeReader(){
  _input = "";
}

void CodeReader::set(KEYPAD *keypad, int pin){
  _keypad = keypad;
  _pin = pin;
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

bool CodeReader::isButton(int position)
{
  if (digitalRead(_pin) == position) {
    delay(1);
    if (digitalRead(_pin) == position) {
      return true;
    }
  }
  return false;
}

void CodeReader::update()
{
  if (_state == DISABLE) {
    return;
  }

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

  if (isButton(HIGH)) {
    _state = TRANSMITTED;
  } 
}
#endif