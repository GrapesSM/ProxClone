#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN 9
#define LED_COUNT 7

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(127, 127, 127));
    strip.show(); //  Update strip to match
    delay(50);
  }

  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(127, 0, 0));
    strip.show(); //  Update strip to match
    delay(50);
  }
}
