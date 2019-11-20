#include "LightGrid.h"
#include <ModbusRtu.h>

#define PIN_RX 0
#define PIN_TX 1
#define PIN_B 2
#define PIN_485_EN 3
#define PIN_A 4
#define PIN_RX1 5
#define PIN_TX1 6
#define PIN_LIMIT 7
#define PIN_NEOPIXEL 9
#define PIN_SPEAKER 11
#define PIN_RELAY1 8
#define PIN_RELAY2 12
#define PIN_GP1
#define PIN_GP2 
#define PIN_GP3
#define PIN_GP4 
#define PIN_GP5
#define PIN_GP6
#define PIN_GP7

#define LED_COUNT 7

Adafruit_NeoPixel strip(LED_COUNT, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
LightGrid lg(&strip, 7);
uint16_t au16data[10] = {2, 127, 127, 127, 127, 0, 0, 0, 127, 0};
Modbus slave(1,0,3);

void setup() {
  slave.begin( 19200 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip.show();            // Turn OFF all pixels ASAP
 }

boolean flag = false;
void loop() {
  slave.poll( au16data, 10 );
  if (!flag) {
    flag = true;
    lg.setColor(au16data[1], au16data[2], au16data[3]);
    strip.show();  
  }

  if (au16data[0] != 3) {
    flag = false;
    au16data[0] = 3;
  }
}
