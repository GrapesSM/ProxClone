#include "ModbusRtu.h"
#include "LightGrid.h"
#include "Constants.h"

Adafruit_NeoPixel strip(LED_COUNT, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
LightGrid lg(&strip, LED_COUNT);
uint16_t au16data[4] = {0, 0, 0, 0};
Modbus slave(1,0,3);

void setup() {
  slave.begin( 19200, PIN_RX1, PIN_TX1 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip.show();            // Turn OFF all pixels ASAP
}

void loop() {
  slave.poll( au16data, 4 ); 
  if (au16data[0] > 0) {
    lg.setColor(au16data[0], au16data[1], au16data[2], au16data[3]);
    strip.show();
    au16data[0] = 0;  
  }
}
