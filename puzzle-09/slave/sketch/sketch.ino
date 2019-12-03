#include "ModbusRtu.h"
#include "LightGrid.h"
#include "Constants.h"
#include <Adafruit_MCP23017.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <ESP32Encoder.h>

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);
LightGrid lg(&strip, LED_COUNT);
uint16_t au16data[4] = {0, 0, 127, 0};
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

  matrix.begin(0x71);

  pinMode(PIN_SWITCH1, INPUT);
  pinMode(PIN_SWITCH2, INPUT);
  pinMode(PIN_SWITCH3, INPUT);

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
  timer1 = millis() - timer1Read;
  slave.poll( au16data, 4 );
  if (au16data[0] > 0) {
    lg.setColor(au16data[0], au16data[1], au16data[2], au16data[3]);
    strip.Show();
    au16data[0] = 0;  
  }

  if (timer1 > 5000) {
    timer1Read = millis();
    Serial.println("==========================");
    Serial.print("Milliseconds: "); 
    Serial.println(millis());
    Serial.print("Switch 1: "); 
    for (int i = 0; i <= 9; i++) {
      Serial.print(mcp1.digitalRead(i));
      if (i < 9) Serial.print(",");  
    }
    Serial.println();
    Serial.print("Switch 2: "); 
    for (int i = 0; i <= 9; i++) {
      Serial.print(mcp2.digitalRead(i));
      if (i < 9) Serial.print(",");  
    }
    Serial.println();
    Serial.print("Neopixel: ");
    for (int i = 0; i < LED_COUNT; i++) {
      Serial.print(strip.GetPixelColor(i).R);
      Serial.print("-");
      Serial.print(strip.GetPixelColor(i).G);
      Serial.print("-");
      Serial.print(strip.GetPixelColor(i).B);
      if (i < LED_COUNT - 1) Serial.print(",");
    }
    Serial.println();
    Serial.print("Seven Segment: "); 
    counter++;
    Serial.print(counter);
    matrix.print(counter);
    matrix.writeDisplay();
    delay(10);
    if (counter > 9999) {
      counter = 0;
    }
    Serial.println();
    Serial.print("Switches: "); 
    Serial.print(digitalRead(PIN_SWITCH1));
    Serial.print(",");
    Serial.print(digitalRead(PIN_SWITCH2));
    Serial.print(",");
    Serial.print(digitalRead(PIN_SWITCH3));
    Serial.println();
    Serial.print("Encoder: ");
    Serial.print(String((int32_t)encoder.getCount()));
    Serial.println(); 
  }
}
