#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include "lib/PrepStatus.h"

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
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

PrepStatus::Components psComponents;

void setupPrepStatus();

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

  // Setup SyncroReader switch
  pinMode(PIN_INPUT_1, INPUT);
  // Setup Power Switch
  pinMode(PIN_SWITCH_1, INPUT);
  
  setupPrepStatus();

  puzzle.timer = millis();
}

void loop()
{  
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );
  
  if (puzzle.registers[1] == 1) {
    psComponents.batteryMatrix.setSolved();
  } else {
    psComponents.batteryMatrix.setSolved(false);
  }

  if (puzzle.registers[2] == 1) {
    psComponents.energySupp.setSolved();
  } else {
    psComponents.energySupp.setSolved(false);
  }

  if (puzzle.registers[3] == 1) {
    psComponents.generator.setSolved();
  } else {
    psComponents.generator.setSolved(false);
  }

  // Enable Energy Supplemental
  PrepStatus::run(psComponents);

  puzzle.timer = millis();
  if (puzzle.timer - puzzle.checkpoint > puzzle.interval) {
    puzzle.checkpoint = millis();
    PrepStatus::show(psComponents);
  }
}

void setupPrepStatus()
{
  psComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitch, PIN_SWITCH_1);
  psComponents.batteryMatrix.set(parts.strip, lightPinsForBatteryMatrix);
  psComponents.energySupp.set(parts.strip, lightPinsForEnergySupp);
  psComponents.generator.set(parts.strip, lightPinsForGenerator);
  psComponents.syncroReader.set(parts.strip, lightPinsForSyncroReader, PIN_INPUT_1);  
//  psComponents.lightEffect.set();
}
