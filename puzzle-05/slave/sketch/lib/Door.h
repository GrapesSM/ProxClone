/*
  Door.h - Library for _______.
*/
#ifndef Door_h
#define Door_h

#include <Arduino.h>
#include <NeoPixelBus.h>

class Door
{
  public:
    Door();
    void set(int pin);
    bool isClosed();
    void open();
    void close();
    void display();
    STATE _state;
  private:
    int _pin;
    bool _closed;
};

Door::Door() {}

void Door::set(int pin) 
{
  _state = OFF;
  _closed = true;
}

bool Door::isClosed()
{
  return _closed;
}

void Door::open()
{
  // TO-DO:
}

void Door::close()
{
  // TO-DO:
}

void Door::display()
{
  // TO-DO:
}

#endif