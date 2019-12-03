
#include <ESP32Encoder.h>

ESP32Encoder encoder;

void setup(){
  
  Serial.begin(115200);
  // Enable the weak pull down resistors
  ESP32Encoder::useInternalWeakPullResistors=false;
  
  // set starting count value
  encoder.clearCount();
  encoder.setCount(16380);

  Serial.println("Encoder Start = "+String((int32_t)encoder.getCount()));

  // Attache pins for use as encoder pins
  encoder.attachHalfQuad(33, 14);
}

void loop(){
  Serial.println("Encoder count = "+String((int32_t)encoder.getCount()));
  delay(100);
}
