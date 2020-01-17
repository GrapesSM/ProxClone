#ifndef BlastDoorKeypad_h
#define BlastDoorKeypad_h

#include <Arduino.h>
#include "CodeReader.h"
#include "Speaker.h"


namespace BlastDoorKeypad{
    typedef struct{
        CodeReader codeReader;
        Speaker speaker;
        STATE state = ON;
    } Components;
    
    void run(Components & c)
    {    
        if(c.state == ON){
            if(c.codeReader.isTransmitted()){
                Serial.println(c.codeReader.getInputKey());
                
                if(c.codeReader.isSolved()){
                     Serial.println("Solved");
                }
                else
                {
                    if(c.codeReader.getInputKey() == keyForCodeReader){
                        c.codeReader.setSolved();
                    }
                    else
                    {
                        Serial.println("Wrong Code");
                        c.codeReader.setTransmitted(false);
                    }
                }
                
            }else{
                c.codeReader.update();
            }
        }
    }

}

#endif