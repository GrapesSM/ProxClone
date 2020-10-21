#include "SoundFiles.h"

void setup() {
  Serial.begin(115200);
}

void loop() {
  
  playSound (9, 0, false, 50); //Conditions: true - play sound: false - sound off
}
