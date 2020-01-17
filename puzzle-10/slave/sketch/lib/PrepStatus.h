/*
  PrepStatus.h - Library for controlling energy and syncro reader.
*/
#ifndef PrepStatus_h
#define PrepStatus_h

#include <Arduino.h>
#include "BatteryMatrix.h"
#include "EnergySupp.h"
#include "LightEffect.h"
#include "Generator.h"
#include "SyncroReader.h"
#include "PowerSwitch.h"
#include "Speaker.h"

namespace PrepStatus {
  typedef struct {
    BatteryMatrix batteryMatrix;
    EnergySupp energySupp;
    LightEffect lightEffect;
    Generator generator;
    SyncroReader syncroReader;
    PowerSwitch powerSwitch;
    Speaker speaker;
    STATE state;
  } Components;

  void run(Components & c) 
  {
    if (c.powerSwitch.isSwitchOff()) {
      c.state = OFF;
    } 
    
    if (c.powerSwitch.isSwitchOn()) {
      c.state = ON;
      c.syncroReader.enable();
    }

    if (c.state == OFF) {
      c.powerSwitch.setLightOff();
    } 
    
    if (c.state == ON) {
      c.powerSwitch.setLightOn();
    }

    if (c.state == ON) {

      if (c.batteryMatrix.isSolved()) {
        c.batteryMatrix.switchToYellow();
      } else {
        c.batteryMatrix.switchToRed();
      }
    
      if (c.energySupp.isSolved()) {
        c.energySupp.switchToYellow();
      } else {
        c.energySupp.switchToRed();
      }
    
      if (c.generator.isSolved()) {
        c.generator.switchToYellow();
      } else {
        c.generator.switchToRed();
      }
    
      if (c.batteryMatrix.isSolved() && c.energySupp.isSolved() && c.generator.isSolved()) {
        c.state = SOLVED;
      } 
      //syncroReader
      if (! c.syncroReader.isDisabled()) {
        if(! c.syncroReader.isSynchronized()){
          c.syncroReader.update();
          if(c.syncroReader.getInputKey() == 1){
            c.syncroReader.setSynchronized();
          }
        }
        else{
          c.syncroReader.setSynchronized();
        }
      }

    }

    if (c.state == SOLVED) {
      c.batteryMatrix.switchToGreen();
      c.energySupp.switchToGreen();
      c.generator.switchToGreen();
    }
  }

  void show(Components & c)
  {
    c.powerSwitch.display();
    // c.batteryMatrix.display();
    // c.energySupp.display();
    // c.generator.display();
    // c.syncroReader.display();
    // c.lightEffect.display();
  }
}

#endif