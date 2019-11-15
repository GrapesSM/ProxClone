#include <Adafruit_NeoPixel.h>
#include <ModbusRtu.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN_NEOPIXEL 9

#define LED_COUNT 7

Adafruit_NeoPixel strip(LED_COUNT, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

uint16_t au16data[10] = {
  3, 127, 127, 127, 127, 0, 0, 0, 127, 0};
Modbus slave(1,0,3);

void setup() {
  slave.begin( 19200 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
  slave.poll( au16data, 10 );
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(au16data[1], au16data[2], au16data[3]));
    strip.show(); //  Update strip to match
    delay(50);
  }

  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(au16data[4], au16data[5], au16data[6]));
    strip.show(); //  Update strip to match
    delay(50);
  }

  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(au16data[7], au16data[8], au16data[9]));
    strip.show(); //  Update strip to match
    delay(50);
  }
}
