
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
    DebounceSwitch _dSwitch;
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
  _dSwitch.set(pin);
}

bool PowerSwitch::isSwitchOn()
{
  return _dSwitch.getState();
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
  _dSwitch.readSwitch();

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