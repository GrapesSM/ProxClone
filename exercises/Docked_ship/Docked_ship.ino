#include <toneAC.h>

#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define addr1 0 //0x20
#define addr2 4 //0x24

// Encoder pins
#define encoder0PinA  2
#define encoder0PinB  4


// Neopixel setup
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
//#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN 9 //neopixel control pin
#define NUMPIXELS 19 // Number of neopixel on the puzzle
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//neopixel commands

//pixels.clear();
//pixels.setPixelColor(i, pixels.Color(0, 150, 0));
//pixels.show();
//pixels.setBrightness(64);
//pixels.fill(32000, 0, 19);

// 7 Segment
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_7segment matrix = Adafruit_7segment();

// Input #0 is on pin 21 so connect a button or switch from there to ground

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;

int mcpRead[14];
int lastmcpRead[14];
int j;
int i;
unsigned int timer1;
unsigned int timer1Read;
int tonefreq = 1000;

volatile int encoder0Pos = 0;
float lcdDisplay;

void setup() {
  Serial.begin(9600);
  tone (10, random(2000,4000), 25);
  // Attach encoder interrupts
  pinMode(encoder0PinA, INPUT);
  digitalWrite(encoder0PinA, HIGH);       // turn on pull-up resistor
  pinMode(encoder0PinB, INPUT);
  digitalWrite(encoder0PinB, HIGH);       // turn on pull-up resistor

  attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2

  // Start i2c expanders
  mcp1.begin(addr1);      // use default address 0
  mcp2.begin(addr2);      // use default address 0


  // 7 segment

  //#ifndef __AVR_ATtiny85__
  //  Serial.begin(9600);
  //  Serial.println("7 Segment Backpack Test");
  //#endif
  matrix.begin(0x71);

  ////////////////////////////
  //#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  //  clock_prescale_set(clock_div_1);
  //#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object
  pixels.setBrightness(6);

  // Setup pins for bottom I2C switches
  mcp1.pinMode(1, INPUT);
  mcp1.pinMode(2, INPUT);
  mcp1.pinMode(3, INPUT);
  mcp1.pinMode(4, INPUT);
  mcp1.pinMode(5, INPUT);
  mcp1.pinMode(6, INPUT);
  mcp1.pinMode(7, INPUT);

  // Setup Pins for upper I2C switches
  mcp2.pinMode(1, INPUT);
  mcp2.pinMode(2, INPUT);
  mcp2.pinMode(3, INPUT);
  mcp2.pinMode(4, INPUT);
  mcp2.pinMode(5, INPUT);
  mcp2.pinMode(6, INPUT);
  mcp2.pinMode(7, INPUT);

  int lastRandom;

  for ( i = 1; i <= 25; i++) {
    lastRandom = random(0, 19);
    pixels.setPixelColor(lastRandom, pixels.Color(random(0, 255), random(0, 255), random(0, 255)));
    pixels.show();
    delay(100);
    pixels.setPixelColor(lastRandom, pixels.Color(0, 0, 0));
    pixels.show();
  }

  pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  pixels.setPixelColor(1, pixels.Color(255, 255, 255));
  pixels.show();

  //reset timers since millis started during setup
  timer1Read =  millis();
  timer1 = 0;
}

void doEncoder() {
  /* If pinA and pinB are both high or both low, it is spinning
     forward. If they're different, it's going backward.

     For more information on speeding up this process, see
     [Reference/PortManipulation], specifically the PIND register.
  */
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0Pos++;
    toneAC (10, random(2000,4000), 25);
    delay(10);
  } else {
    encoder0Pos--;
    tone (10, random(2000,4000), 25);
    delay(10);
  }

}

void doEncoder_Expanded() {
  if (digitalRead(encoder0PinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoder0PinB) == LOW) {  // check channel B to see which way
      // encoder is turning
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
    else {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  {
    if (digitalRead(encoder0PinB) == LOW) {   // check channel B to see which way
      // encoder is turning
      encoder0Pos = encoder0Pos + 1;          // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }

  }

}

void loop() {

  if (encoder0Pos <= 0) {
    encoder0Pos = 0;
    matrix.println(lcdDisplay / 10, 1);
  } else if (encoder0Pos >= 1010) {
    encoder0Pos = 1010;
  }
  lcdDisplay = encoder0Pos;
  matrix.println(lcdDisplay / 10, 1);
  matrix.writeDisplay();


  // Countdown timer for sync key

  timer1 =  millis() - timer1Read;

  if ((timer1 >= 0) && (timer1 <= 1000)) {
    pixels.setPixelColor(4, pixels.Color(0, 255, 0));
    pixels.show();
  } else if ((timer1 >= 1000) && (timer1 <= 2000)) {
    pixels.setPixelColor(4, pixels.Color(0, 0, 0));
    pixels.show();
  } else if ((timer1 >= 2000) && (timer1 <= 3000)) {
    pixels.setPixelColor(3, pixels.Color(0, 255, 0));
    pixels.show();
  } else if  ((timer1 >= 3000) && (timer1 <= 4000)) {
    pixels.setPixelColor(3, pixels.Color(0, 0, 0));
    pixels.show();
  } else if ((timer1 >= 4000) && (timer1 <= 5000)) {
    pixels.setPixelColor(2, pixels.Color(0, 255, 0));
    pixels.show();
  } else if  ((timer1 >= 5000) && (timer1 <= 6000)) {
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.show();
  } else if  ((timer1 >= 6000) && (timer1 >= 7000)) {
    timer1Read = millis();
    timer1 = 0;
  }



  for ( i = 1; i <= 14; i++) {
    if (i <= 7) {
      mcpRead[i] = mcp1.digitalRead(i);
      if (lastmcpRead[i] != mcpRead[i]) {
        //Serial.print(lastmcpRead[i]); Serial.print("   "); Serial.println(mcpRead[i]);
        tone (10, 4000, 100);
      }
      lastmcpRead[i] = mcpRead[i];

      if (mcp1.digitalRead(i) == HIGH) {

        //        mcpRead[i] = mcp1.digitalRead(i);
        pixels.setPixelColor((NUMPIXELS - 7) - i, pixels.Color(255, 0, 0));
        pixels.show();
      } else {
        mcpRead[i] = 0;
        pixels.setPixelColor((NUMPIXELS - 7) - i, pixels.Color(0, 0, 0));
        pixels.show();
      }
    }
    if (i >= 8) {
      if (mcp2.digitalRead(i - 7) == HIGH) {
        pixels.setPixelColor((NUMPIXELS + 7) - i, pixels.Color(255, 0, 0));
        pixels.show();
      } else {
        pixels.setPixelColor((NUMPIXELS + 7) - i, pixels.Color(0, 0, 0));
        pixels.show();
      }
    }
  }
  i = 0;

  if ((mcpRead[1] == 1) && (mcpRead[2] == 1)) {
    pixels.fill(32000, 0, 19);
    //pixels.clear();
    pixels.show();
    matrix.writeDigitRaw (0, B01110011); //P
    matrix.writeDigitRaw (1, B00111000); //l
    matrix.writeDigitRaw (3, B01110111); //A
    matrix.writeDigitRaw (4, B01101110); //y
    matrix.writeDisplay();
    while (1) {}
  }

}
