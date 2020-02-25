#ifndef BlastDoorKeypad_h
#define BlastDoorKeypad_h

#include <Arduino.h>
#include "CodeReader.h"
#include "Speaker.h"

namespace BlastDoorKeypad {
  typedef struct
  {
    CodeReader codeReader;
    Speaker speaker;
    STATE state = ON;
  } Components;

  void run(Components &c)
  {
    if (c.state == ON)
    {
      if (c.codeReader.isTransmitted())
      {
        if (c.codeReader.isSolved())
        {
        }
        else
        {
          if (c.codeReader.getInputKey() == keyForCodeReader)
          {
            c.codeReader.setSolved();
          }
          else
          {
            c.codeReader.setTransmitted(false);
          }
        }
      }
      else
      {
        c.codeReader.update();
      }
    }
  }
} // namespace BlastDoorKeypad

#endif