#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include <Adafruit_MCP23017.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <ESP32Encoder.h>
#include "lib/EnergySupplemental.h"
#include "lib/ShipPrepAux.h"
#include "sounds/soundPowerUp.h"
// #include "sounds/soundPowerDown.h"
// #include "sounds/soundKeyInsert.h"

Puzzle puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  Adafruit_MCP23017 mcp1;
  Adafruit_MCP23017 mcp2;
  ESP32Encoder encoder;
  Adafruit_7segment matrix = Adafruit_7segment();
  unsigned char* listOfSounds[NUMBER_OF_SOUNDS];
  unsigned int listOfLengthOfSounds[NUMBER_OF_SOUNDS];
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

EnergySupplemental::Components esComponents;
ShipPrepAux::Components spComponents;

void setupEnergySupplemental();
void setupShipPrepAux();

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
  ESP32Encoder::useInternalWeakPullResistors = false;
  //-- adjust starting count value to 0
  parts.encoder.clearCount();
  parts.encoder.setCount(0);
  //-- attach pins for use as encoder pins
  parts.encoder.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);

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

  // Setup speaker pins
  pinMode(PIN_SPEAKER, OUTPUT);
  ledcSetup(PWM_SPEAKER_CHANNEL, PWM_SPEAKER_FREQUENCY, PWM_SPEAKER_RESOLUTION);
  ledcAttachPin(PIN_SPEAKER, PWM_SPEAKER_CHANNEL);
  pinMode(PIN_AMPLIFIER, OUTPUT);
  digitalWrite(PIN_AMPLIFIER, HIGH);

  // Setup sound list
  parts.listOfSounds[SOUND_POWER_UP] = soundPowerUp;
  parts.listOfLengthOfSounds[SOUND_POWER_UP] = sizeof(soundPowerUp)/sizeof(soundPowerUp[0]);
  // parts.listOfSounds[SOUND_POWER_DOWN] = soundPowerDown;
  // parts.listOfLengthOfSounds[SOUND_POWER_DOWN] = sizeof(soundPowerDown)/sizeof(soundPowerDown[0]);
  // parts.listOfSounds[SOUND_KEY_INSERT] = soundKeyInsert;
  // parts.listOfLengthOfSounds[SOUND_KEY_INSERT] = sizeof(soundKeyInsert)/sizeof(soundKeyInsert[0]);

  setupEnergySupplemental();
  setupShipPrepAux();

  esComponents.state = SETUP;
  spComponents.state = SETUP;
}

void loop()
{
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );

  EnergySupplemental::update(puzzle, esComponents);
  ShipPrepAux::update(puzzle, spComponents);
  
  EnergySupplemental::run(esComponents);
  ShipPrepAux::run(spComponents);

  EnergySupplemental::show(esComponents);
  ShipPrepAux::show(spComponents);
}

void setupEnergySupplemental()
{
  esComponents.powerAdjuster.set(&parts.encoder, &parts.matrix);
  esComponents.syncroReader.set(parts.strip, lightPinsForSyncroReader, PIN_INPUT_1);
  esComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitchOfEnergySupplemental, PIN_SWITCH_1);
  esComponents.speaker.set(PIN_SPEAKER, PIN_AMPLIFIER, 65, parts.listOfSounds, parts.listOfLengthOfSounds);
}

void setupShipPrepAux()
{
  spComponents.batteryMatrix.set(parts.strip, lightPinsForBatteryMatrix, &parts.mcp2, switchPinsForBatteryMatrix, labelsForBatteryMatrix);
  spComponents.generator.set(parts.strip, lightPinsForGenerator, &parts.mcp1, switchPinsForGenerator, labelsForGenerator);
  spComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitchOfShipPrep, PIN_SWITCH_2);
  spComponents.speaker.set(PIN_SPEAKER, PIN_AMPLIFIER, 65, parts.listOfSounds, parts.listOfLengthOfSounds);
}
