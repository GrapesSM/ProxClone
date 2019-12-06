#include "Constants.h"
#include "lib/ModbusRtu.h"
#include <Adafruit_MCP23017.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <ESP32Encoder.h>
#include "EnergySupplemental.h"
#include "ShipPrepAux.h"

struct Puzzle {
  uint8_t address = ADDR_SLAVE;
  STATE state = INITIALIZED;
  bool forced = false;
  int totalPower = 10;
  uint8_t numberOfRegisters = 20;
  uint16_t registers[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  unsigned long startTime = 0;
  unsigned long endTime = 0;
  unsigned long timer = 0;
  unsigned long counter = 0;
  unsigned long checkpoint = 0;
} puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  Adafruit_MCP23017 mcp1;
  Adafruit_MCP23017 mcp2;
  ESP32Encoder encoder;
  Adafruit_7segment matrix = Adafruit_7segment();  
} parts;

void setupEnergySupplemental();
void setupShipPrepAux();

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);

  // Setup Modbus communication
  Modbus slave(puzzle.address, 1, PIN_485_EN);
  parts.slave = &slave;
  parts.slave->begin( SERIAL_BAUDRATE, PIN_RX_1, PIN_TX_1 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  // Setup NeoPixelBus for all lights
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);
  parts.strip = &strip;
  parts.strip->Begin();
  parts.strip->Show();

  // Setup and Init Encoders
  ESP32Encoder::useInternalWeakPullResistors=false;
  //-- attach pins for use as encoder pins
  parts.encoder.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);
  //-- adjust starting count value to 0
  parts.encoder.clearCount();
  parts.encoder.setCount(16380);
  
  // Setup 7 segment LED
  parts.matrix.begin(ADDR_SEVENSEGMENT);
  
  // Setup power switch
  pinMode(PIN_SWITCH_1, INPUT);
  pinMode(PIN_SWITCH_2, INPUT);
  pinMode(PIN_SWITCH_3, INPUT);

  // Setup pins for bottom I2C switches
  parts.mcp1.begin(ADDR_SWITCH_1);
  for (uint8_t i = 1; i <= NUMBER_OF_SWITCHES_1; i++) {
    parts.mcp1.pinMode(i, INPUT);
  }

  // Setup Pins for upper I2C switches
  parts.mcp2.begin(ADDR_SWITCH_2);
  for (uint8_t i = 1; i <= NUMBER_OF_SWITCHES_2; i++) {
    parts.mcp2.pinMode(i, INPUT);
  }
  
  setupEnergySupplemental();
  setupShipPrepAux();

  puzzle.timer = millis();
}

void loop() 
{
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );
}

void setupEnergySupplemental()
{
  PowerAdjuster pa(parts.encoder, parts.matrix);
  SyncroReader sr(parts.strip, {2, 3, 4});
  PowerSwitch ps(parts.strip, {1}, PIN_SWITCH_1);
  Speaker sp;

  EnergySupplemental::Components esComponents = {
    .powerAdjuster = &pa,
    .syncroReader = &sr,
    .powerSwitch = &ps,
    .speaker = &sp
  }
}

void setupShipPrepAux() 
{
  BatteryMatrix bm(parts.strip, {12, 13, 14, 15, 16, 17, 18}, parts.mcp2);
  Generator ge(parts.strip, {5, 6, 7, 8, 9, 10, 11}, parts.mcp1);
  PowerSwitch ps(parts.strip, {19}, PIN_SWITCH_2);
  Speaker sp;

  ShipPrepAux::Components spComponents = {
    .batteryMatrix = &bm;
    .generator = &ge;
    .powerSwitch = &ps;
    .speaker = &sp;
  };
}
