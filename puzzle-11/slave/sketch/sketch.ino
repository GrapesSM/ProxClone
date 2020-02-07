#include "Constants.h"
#include "SoundData.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include "lib/LaserGrid.h"

struct Puzzle {
  uint8_t address = ADDR_SLAVE;
  STATE state = NOT_INITIALIZED;
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
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

LaserGrid::Components lgComponents;

void setupLaserGrid();

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);

  // Setup Modbus communication
  parts.slave = &slave;
  parts.slave->begin( SERIAL_BAUDRATE, PIN_RX_1, PIN_TX_1 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  // Setup NeoPixelBus for all lights
  parts.strip = &strip;
  parts.strip->Begin();
  parts.strip->Show();

  // Setup Nextion Display
  Serial2.begin(SERIAL2_BAUDRATE, SERIAL_8N1, PIN_RX_2, PIN_TX_2);
  
  // Setup Switches
  pinMode(PIN_SWITCH_1, INPUT);
  pinMode(PIN_SWITCH_2, INPUT);
  pinMode(PIN_SWITCH_3, INPUT);

  // Setup Inputs
  pinMode(PIN_INPUT_1, INPUT);
  pinMode(PIN_INPUT_2, INPUT);
  pinMode(PIN_INPUT_3, INPUT);

  // Setup Potentiometers 
  pinMode(PIN_ANALOG_INPUT_1, INPUT);
  pinMode(PIN_ANALOG_INPUT_2, INPUT);
  pinMode(PIN_ANALOG_INPUT_3, INPUT);

  // Setup solenoid
  pinMode(PIN_RELAY_1, OUTPUT);

  setupLaserGrid();

  puzzle.timer = millis();
}

void loop()
{
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );

  // Enable Laser Grid
  LaserGrid::run(lgComponents);

  puzzle.timer = millis();
  if (puzzle.timer - puzzle.checkpoint > puzzle.interval) {
    puzzle.checkpoint = millis();
    LaserGrid::show(lgComponents);
  }

  ///////////////////////////////
  puzzle.registers[5] = lgComponents.state;
  puzzle.registers[6] = lgComponents.powerSwitch.getState();
  puzzle.registers[7] = lgComponents.keyReader.getState();
  puzzle.registers[8] = lgComponents.waveAdjuster.getState();
}

void setupLaserGrid()
{
  lgComponents.waveAdjuster.set(PIN_ANALOG_INPUT_1, PIN_ANALOG_INPUT_2, PIN_ANALOG_INPUT_3, &Serial2);
  lgComponents.keyReader.set(PIN_INPUT_1, PIN_INPUT_2, PIN_INPUT_3, PIN_RELAY_1);
  lgComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitch, PIN_SWITCH_1);
//  lgComponents.speaker.set(parts.dacAudio, PIN_AMPLIFIER);
}
