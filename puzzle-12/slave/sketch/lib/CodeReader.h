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
        void disable();
        void enable();
        void update();
        bool isDisabled();
        String getInputKey();
        bool isSolved();
        void setSolved(bool);
        bool isTransmitted();
        void setTransmitted(bool);
    private:       
        bool _solved;
        bool _disabled;
        String _input;
        bool _transmitted;
        int _pin;
        KEYPAD *_keypad;
};

CodeReader::CodeReader(){
    _disabled = true;
    _input = "";
    _solved = false;
    _transmitted = false;
}

void CodeReader::set(KEYPAD *keypad, int pin){
    _keypad = keypad;
    _pin = pin;
}

bool CodeReader::isSolved() {
  return _solved;
}
void CodeReader::setSolved(bool solved = true) {
  _solved = solved;
}

bool CodeReader::isTransmitted() {
  return _transmitted;
}

void CodeReader::setTransmitted(bool transmitted = true) {
  _transmitted = transmitted;
  if(transmitted == false){
    _input = "";
  }
}


String CodeReader::getInputKey() {
  return _input;
}

void CodeReader::disable()
{
  _disabled = true;
}

void CodeReader::enable()
{
  _disabled = false;
}

bool CodeReader::isDisabled()
{
  return _disabled;
}
void CodeReader::update()
{
  _keypad->updateFIFO();  // necessary for keypad to pull button from stack to readable register
  char button = _keypad->getButton();

  if (button == -1)
  {
    Serial.println("No keypad detected");
    return;
  }
  
  if (button != 0)
  {
    _input += button;
    if(button == '*'){
      setTransmitted();
    }
    Serial.println(_input);
    if(_input.length() >= 10){
      _input = "";
    }
  }
  //Do something else. Don't call your Keypad a ton otherwise you'll tie up the I2C bus
  delay(25); //25 is good, more is better  
}
#endif