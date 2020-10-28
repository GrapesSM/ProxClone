/*
  Speaker.h - Library for playing sounds and voices.
*/
#ifndef Speaker_h
#define Speaker_h

#include <Arduino.h>

#define buffersize 1024

char buffer[buffersize];

class Speaker
{
  public:
    Speaker();
    void init();
    void set(String soundFilenames[]);
    void setState(STATE state);
    STATE getState();
    void update();
    void setPlayPartly(bool value);
    bool getPlayPartly();
    void setRepeat(bool value);
    bool getRepeat();
    void soundOff();
    void setCurrent(int soundNumber, int microseconds);
    void play(uint8_t volume);
    void playBytes(int numberOfBytes, uint8_t volume);
  private:
    File _f;
    int _microseconds;
    int _soundNumber;
    bool _repeat;
    bool _playPartly;
    String* _soundFilenames;
    STATE _state;
};

Speaker::Speaker() 
{
  init();
}

void Speaker::init()
{
  _soundNumber = 0;
  _microseconds = 5;
  _repeat = false;
  _playPartly = false;
}

void Speaker::set(String soundFilenames[]) {
  _soundFilenames = soundFilenames;
}

void Speaker::setState(STATE state)
{
  _state = state;;
}

STATE Speaker::getState()
{
  return _state;
}

void Speaker::update()
{
    
}

void Speaker::setPlayPartly(bool value)
{
  _playPartly = value;
}

bool Speaker::getPlayPartly()
{
  return _playPartly;
}

void Speaker::setRepeat(bool value)
{
  _repeat = value;
}

bool Speaker::getRepeat() 
{
  return _repeat;
}

void Speaker::soundOff()
{
  if (_f) {
    _f.seek(_f.size() - 1);
    _f.close();
  }
}

void Speaker::setCurrent(int soundNumber, int microseconds = 20)
{
  _soundNumber = soundNumber;
  _microseconds = microseconds;
  _f = SPIFFS.open(_soundFilenames[_soundNumber], "r");
}

void Speaker::play(uint8_t volume = 30)
{
  if (_state == DISABLE) {
    soundOff();
    return;
  }

  if (!_f) {
    return;
  }

  int byteflag = 0;
  int i;
  unsigned int timer1old;

  while (_f && _f.position() < (_f.size() - 1)) {
    int numBytes = _min(1024, _f.size() - _f.position() - 1); // f.size() / 16000 ~ X seconds ~ X * 1000 milliseconds

    if (byteflag == 0) {
      _f.readBytes(buffer, numBytes);
      byteflag = 1;
    }

    if (i < numBytes) {
      int old = micros();
      i++;
      dacWrite(PIN_SPEAKER, map((unsigned int)buffer[i], 0, 255, 0, volume));
      timer1old = micros();
      while (micros() - old < _microseconds); //125usec = 1sec/8000 and assume 5us for overhead like wifi
    } else {
      i = 0;
      byteflag = 0;
    }
  }

  byteflag = 0;
  digitalWrite(PIN_SPEAKER,LOW);

  if (_f) 
    _f.close();
}

void Speaker::playBytes(int numberOfBytes = 1024, uint8_t volume = 30)
{
  if (_state == DISABLE) {
    return;
  }

  if (!_f) {
    return;
  }

  int byteflag = 0;
  int i;
  unsigned int timer1old;

  if (_repeat && _f.position() >= _f.size() - 1) {
    _f.seek(0);
  }

  if (_f && _f.position() < (_f.size() - 1)) {
    int numBytes = _min(numberOfBytes, _f.size() - _f.position() - 1); // f.size() / 16000 ~ X seconds ~ X * 1000 milliseconds

    if (byteflag == 0) {
      _f.readBytes(buffer, numBytes);
      byteflag = 1;
    }

    if (i < numBytes) {
      int old = micros();
      i++;
      dacWrite(PIN_SPEAKER, map((unsigned int)buffer[i], 0, 255, 0, volume));
      timer1old = micros();
      while (micros() - old < _microseconds); //125usec = 1sec/8000 and assume 5us for overhead like wifi
    } else {
      i = 0;
      byteflag = 0;
    }
  }

  byteflag = 0;
  digitalWrite(PIN_SPEAKER,LOW);
}

#endif