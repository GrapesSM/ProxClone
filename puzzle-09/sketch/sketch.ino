#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN 9
#define LED_COUNT 7

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
