/*
  Speaker.h - Library for playing sounds and voices.
*/
#ifndef Speaker_h
#define Speaker_h

#include <Arduino.h>
#include <XT_DAC_Audio.h>

class Speaker
{
  public:
    Speaker();
    void set(XT_DAC_Audio_Class * dacAudio, int enablePin);
    void setFrequency(int frequency = PWM_FREQUENCY);
    void setDutycycle(int dutycycle = PWM_DUTYCYCLE);
    void init();
    void update();
    void enable();
    void disable();
    void isDisabled();
  private:
    XT_DAC_Audio_Class * _dacAudio;
    int _enablePin;
    bool _disabled;
    int _counter;
    XT_PlayListItem_Class* _playlist;
};

Speaker::Speaker() {}

void Speaker::set(XT_DAC_Audio_Class * dacAudio, int enablePin)
{
  _dacAudio = dacAudio;
  _enablePin = enablePin;
  _disabled = true;
}

void Speaker::enable()
{
  _disabled = false;
}

void Speaker::disable()
{
  _disabled = true;
}

void Speaker::update()
{
  
}

#endif