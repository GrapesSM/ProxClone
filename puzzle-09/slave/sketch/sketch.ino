#include "ModbusRtu.h"
#include "LightGrid.h"
#include "Constants.h"
#include <Adafruit_MCP23017.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <ESP32Encoder.h>

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);
LightGrid lg(&strip, LED_COUNT);
uint16_t au16data[70] = {
  0, 0, 0, 0, 0, 0, 0, 0, 127, 127,
  127, 0, 0, 127, 0, 0, 127, 0, 0, 127,
  127, 0, 0, 127, 0, 0, 127, 0, 0, 127,
  0, 0, 127, 0, 0, 127, 0, 0, 127, 0,
  0, 0, 127, 0, 0, 127, 0, 0, 127, 0,
  0, 127, 0, 0, 127, 0, 0, 127, 0, 0,
  127, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

Modbus slave(1, 1, PIN_485_EN);
ESP32Encoder encoder;

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;

Adafruit_7segment matrix = Adafruit_7segment();
uint16_t counter = 0;

unsigned int timer1;
unsigned int timer1Read;

void setup() {
  Serial.begin(19200);
  
  slave.begin( 19200, 2, 4 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.Begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.Show();             // Turn OFF all pixels ASAP

  mcp1.begin(ADDR_SWITCH1);
  mcp2.begin(ADDR_SWITCH2);
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

  //--- Setup 7 segment LED
  matrix.begin(0x71);

  pinMode(PIN_SWITCH1, INPUT);
  pinMode(PIN_SWITCH2, INPUT);
  pinMode(PIN_SWITCH3, INPUT);

  //--- Setup and Init Encoders
  ESP32Encoder::useInternalWeakPullResistors=false;
  // adjust starting count value to 0
  encoder.clearCount();
  encoder.setCount(16380);
  // Attache pins for use as encoder pins
  encoder.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);
  
  // reset timers since millis started during setup
  timer1Read =  millis();
  timer1 = 0;
}

void loop() {
  slave.poll( au16data, 70 );
  
  timer1 = millis() - timer1Read;
  if (counter > 9999) {
    counter = 0;
  }

  if (au16data[0] == 1) {
    for (int i = 1; i <= 18; i++) {
      if (i >= 5) {
        if (i <= 11) {
          if (mcp1.digitalRead(12 + i*(-1))) {
            lg.setColor(i, au16data[8 + 3*(i-1)], au16data[8 + 3*(i-1) + 1], au16data[8 + 3*(i-1) + 2]);              
          } else {
            lg.setColor(i, 0, 0, 0); 
          }
        } else {
          if (mcp2.digitalRead(19 + i*(-1))) {
            lg.setColor(i, au16data[8 + 3*(i-1)], au16data[8 + 3*(i-1) + 1], au16data[8 + 3*(i-1) + 2]);              
          } else {
            lg.setColor(i, 0, 0, 0); 
          }
        }
      } else if (i == 1) {
        if (au16data[5]) {
          lg.setColor(i, au16data[8 + 3*(i-1)], au16data[8 + 3*(i-1) + 1], au16data[8 + 3*(i-1) + 2]);
        } else {
          lg.setColor(i, 0, 0, 0);    
        }
      } else {
        lg.setColor(i, au16data[8 + 3*(i-1)], au16data[8 + 3*(i-1) + 1], au16data[8 + 3*(i-1) + 2]);    
      }
    }
    au16data[0] = 0;
    strip.Show();
  }
  
  /////////////////////////////////////////
  byte sw1 = 0x00000000;
  for (int i = 7; i > 0; i--) {
    mcp1.digitalRead(i) ? bitSet(sw1, 7 - i) : bitClear(sw1, 7 - i);
  }
  au16data[1] = sw1;

  byte sw2 = 0x00000000;
  for (int i = 7; i > 0; i--) {
    mcp2.digitalRead(i) ? bitSet(sw2, 7 - i) : bitClear(sw2, 7 - i);
  }
  au16data[2] = sw2;

  au16data[3] = counter;
  au16data[4] = (int16_t)encoder.getCount();
  au16data[5] = digitalRead(PIN_SWITCH1);
  au16data[6] = digitalRead(PIN_SWITCH2);
  au16data[7] = digitalRead(PIN_SWITCH3);

  if (timer1 > 300) {
    counter++;
    timer1Read = millis();
    au16data[0] = 1;
    if (au16data[4] >= 0 && au16data[4] <= 9999) {
      matrix.print(au16data[4]);  
    } else if (au16data[4] > 9999 && au16data[4] < 10100) {
      au16data[4] = 9999;
    } else {
      au16data[4] = 0;
    }
    
    matrix.writeDisplay();
  }
}
