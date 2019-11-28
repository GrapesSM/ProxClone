#include "ModbusRtu.h"
#include "LightGrid.h"
#include "Constants.h"

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);
LightGrid lg(&strip, LED_COUNT);
uint16_t au16data[4] = {0, 127, 127, 0};
Modbus slave(1, 1, PIN_485_EN);

void setup() {
  slave.begin( 19200, 2, 4 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.Begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.Show();             // Turn OFF all pixels ASAP
}

void loop() {
  slave.poll( au16data, 4 ); 
  if (au16data[0] > 0) {
    lg.setColor(au16data[1], au16data[2], au16data[3]);
    strip.Show();
    au16data[0] = 0;  
  }
}
