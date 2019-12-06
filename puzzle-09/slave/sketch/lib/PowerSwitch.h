/*
  PowerSwitch.h - Library for playing sounds and voices.
*/
#ifndef PowerSwitch_h
#define PowerSwitch_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class PowerSwitch
{
  public:
    PowerSwitch(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, uint8_t lightPins[], uint8_t switchPin);
    void setLightOn();
    void setLightOff();
    void switchOn();
    void switchOff();
    bool isLightOn();
    bool isLightOff();
    bool isSwitchOn();
    bool isSwitchOff();

  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * _strip;
    uint8_t _lightPins;
    uint8_t _pin;
};

PowerSwitch::PowerSwitch(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip, uint8_t lightPins[], uint8_t switchPin) {
  _strip = strip;
  _lightPins = lightPins;
  _pin = pin;
}

void PowerSwitch::setLightOn() {
  strip.set
  
}

#endif