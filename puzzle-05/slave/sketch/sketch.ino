#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include <ESP32Encoder.h>
#include "lib/Safeomatic.h"

Puzzle puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  ESP32Encoder encoder;
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

Safeomatic::Components smComponents;

void setupSafeomatic();

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

  // Setup and Init Encoders
  ESP32Encoder::useInternalWeakPullResistors=false;
  //-- adjust starting count value to 0
  parts.encoder.clearCount();
  parts.encoder.setCount(0);
  //-- attach pins for use as encoder pins
  parts.encoder.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);
   
  // Setup power switch
  pinMode(PIN_INPUT_1, INPUT);
  pinMode(PIN_RELAY_1, OUTPUT);
  digitalWrite(PIN_RELAY_1, LOW);
  pinMode(PIN_RELAY_2, OUTPUT);
  digitalWrite(PIN_RELAY_2, LOW);

  setupSafeomatic();

  puzzle.timer = millis();
}

void loop()
{
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );

  // Enable Datamatic
  Safeomatic::run(smComponents);

  puzzle.timer = millis();
  if (puzzle.timer - puzzle.checkpoint > puzzle.interval) {
    puzzle.checkpoint = millis();
    Safeomatic::show(smComponents);
  }
}

void setupSafeomatic()
{
  smComponents.combinationReader.set(parts.strip, &parts.encoder);
  smComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitch, PIN_SWITCH_1);
  //smComponents.speaker.set(&parts.speaker);
  smComponents.accessPanel.set(PIN_INPUT_1, PIN_RELAY_1);
  smComponents.door.set(parts.strip, safeLightPin, PIN_RELAY_2);
}
