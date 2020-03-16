#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include "lib/StatusBoard.h"

Puzzle puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  Adafruit_7segment matrix = Adafruit_7segment(); 
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

StatusBoard::Components sbComponents;

void setupStatusBoard();

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

  // Setup 7 segment LED
  parts.matrix.begin(ADDR_SEVENSEGMENT);

  // Setup speaker pins
  pinMode(PIN_SPEAKER, OUTPUT);
  ledcSetup(PWM_SPEAKER_CHANNEL, PWM_SPEAKER_FREQUENCY, PWM_SPEAKER_RESOLUTION);
  ledcAttachPin(PIN_SPEAKER, PWM_SPEAKER_CHANNEL);
  pinMode(PIN_AMPLIFIER, OUTPUT);
  digitalWrite(PIN_AMPLIFIER, HIGH);
  
  setupStatusBoard();

  sbComponents.state = SETUP;
}

void loop() 
{
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );

  // Map puzzle register's to component's values
  StatusBoard::update(sbComponents);

  // State changes
  StatusBoard::run(sbComponents);

  // Show changes
  StatusBoard::show(sbComponents);
}

void setupStatusBoard()
{
  sbComponents.shipPrepStatus.set(parts.strip, lightPinsForShipPrepStatus);
  sbComponents.lifeSupportStatus.set(parts.strip, lightPinsForLifeSupportStatus);
  sbComponents.blastDoorStatus.set(parts.strip, lightPinsForBlastDoorStatus);
  sbComponents.laserGrid.set(parts.strip, lightPinsForLaserGridStatus);
}