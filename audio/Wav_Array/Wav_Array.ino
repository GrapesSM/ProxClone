#include "SoundFiles.h"

void setup() {
  Serial.begin(115200);
  
}

void loop() {
  
  playSound (0, 0, true, 50); //Conditions: true - play sound: false - sound off
}
