
/*
  PowerSwitch.h
*/
#ifndef PowerSwitch_h
#define PowerSwitch_h

#include <Arduino.h>

class PowerSwitch
{
  public:
    PowerSwitch();
    void init();
    void set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin, int pin);
    void setLightOn();
    void setLightOff();
    void display();
    void update();
    void setState(STATE state);
    STATE getState();
    void readSwitch();
    bool isSwitchOn();
    bool isSwitchOff();

  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    int _lightPin;
    int _pin;
    int _reading;
    int _switchState;
    int _lastSwitchState;
    unsigned long _lastDebounceTime = 0;
    unsigned long _debounceDelay = 50;
    STATE _state;
};

PowerSwitch::PowerSwitch() 
{
  init();
}

void PowerSwitch::init()
{
  
}

void PowerSwitch::set(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip, int lightPin, int pin) 
{
  _strip = strip;
  _lightPin = lightPin;
  _pin = pin;
}

void PowerSwitch::readSwitch()
{
  _reading = digitalRead(_pin);
  if (_reading != _lastSwitchState) {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    // if (_reading != _switchState) {
      _switchState = _reading;
    // }
  }

  _lastSwitchState = _reading;
}


bool PowerSwitch::isSwitchOn()
{
  return _switchState;
}

bool PowerSwitch::isSwitchOff()
{
  return ! isSwitchOn();
}

void PowerSwitch::setLightOn()
{
  _strip->SetPixelColor(_lightPin, RgbColor(127, 127, 127));
}

void PowerSwitch::setLightOff()
{
  _strip->SetPixelColor(_lightPin, RgbColor(0, 0, 0));
}

void PowerSwitch::display()
{
  _strip->Show();
}

void PowerSwitch::update() 
{
  readSwitch();

  switch (_state)
  {
    case DISABLE:
      setLightOff();
      break;
    
    default:
      if (isSwitchOff()) {
        _state = OFF;
        setLightOff();
      }

      if (isSwitchOn()) {
        _state = ON;
        setLightOn();
      }
      break;
  }
}

void PowerSwitch::setState(STATE state)
{
  _state = state;
}

STATE PowerSwitch::getState()
{
  return _state;
}

#endif