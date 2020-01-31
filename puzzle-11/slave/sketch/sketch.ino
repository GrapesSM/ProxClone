#include "Constants.h"
#include "lib/ModbusRtu.h"
//#include "NeoPixelBus.h"
//#include <Adafruit_MCP23017.h>
//#include <Adafruit_GFX.h>
//#include "Adafruit_LEDBackpack.h"
//#include <ESP32Encoder.h>
//#include "lib/LaserGrid.h"

struct Puzzle {
  uint8_t address = ADDR_SLAVE;
  STATE state = INITIALIZED;
  bool forced = false;
  int totalPower = 10;
  uint8_t numberOfRegisters = 10;
  uint16_t registers[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  unsigned long startTime = 0;
  unsigned long endTime = 0;
  unsigned long timer = 0;
  unsigned long counter = 0;
  unsigned long checkpoint = 0;
  unsigned long interval = 200;
} puzzle;

struct Parts {
  Modbus * slave;
//  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
//  ESP32Encoder encoder1;
//  ESP32Encoder encoder2;
//  ESP32Encoder encoder3;
//  Adafruit_7segment matrix = Adafruit_7segment();
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);
//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

//LaserGrid::Components lgComponents;

//void setupLaserGrid();

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);

  // Setup Modbus communication
  parts.slave = &slave;
  parts.slave->begin( SERIAL_BAUDRATE, PIN_RX_1, PIN_TX_1 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

//  // Setup NeoPixelBus for all lights
//  parts.strip = &strip;
//  parts.strip->Begin();
//  parts.strip->Show();
//
//  // Setup and Init Encoders
//  ESP32Encoder::useInternalWeakPullResistors=false;
//  //-- adjust starting count value to 0
//  parts.encoder1.clearCount();
//  parts.encoder1.setCount(0);
//  //-- attach pins for use as encoder pins
//  parts.encoder1.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);
//
//  // Setup and Init Encoders
//  ESP32Encoder::useInternalWeakPullResistors=false;
//  //-- adjust starting count value to 0
//  parts.encoder2.clearCount();
//  parts.encoder2.setCount(0);
//  //-- attach pins for use as encoder pins
//  parts.encoder2.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);
//
//  // Setup and Init Encoders
//  ESP32Encoder::useInternalWeakPullResistors=false;
//  //-- adjust starting count value to 0
//  parts.encoder3.clearCount();
//  parts.encoder3.setCount(0);
//  //-- attach pins for use as encoder pins
//  parts.encoder3.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);
//  
//  // Setup 7 segment LED
//  parts.matrix.begin(ADDR_SEVENSEGMENT);
//  
//  // Setup power switch
//  pinMode(PIN_SWITCH_1, INPUT);
//  pinMode(PIN_SWITCH_2, INPUT);
//  pinMode(PIN_SWITCH_3, INPUT);
//
//  setupLaserGrid();

  puzzle.timer = millis();
}

void loop()
{
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );

//  // Enable Laser Grid
//  LaserGrid::run(lgComponents);
//
//  puzzle.timer = millis();
//  if (puzzle.timer - puzzle.checkpoint > puzzle.interval) {
//    puzzle.checkpoint = millis();
//    LaserGrid::show(lgComponents);
//  }
}

//void setupLaserGrid()
//{
//  lgComponents.waveAdjuster.set(parts.encoder1, parts.encoder2, parts.encoder3, parts.matrix);
//  lgComponents.keyReader.set();
//  lgComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitch, PIN_SWITCH_1);
//}
