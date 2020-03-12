#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include "lib/LaserGrid.h"
#include "sounds/soundPowerUp.h"
#include "sounds/soundPowerDown.h"
#include "sounds/soundKeyInsert.h"

Puzzle puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  unsigned char* listOfSounds[NUMBER_OF_SOUNDS];
  unsigned int listOfLengthOfSounds[NUMBER_OF_SOUNDS];
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

  // Setup speaker pins
  pinMode(PIN_SPEAKER, OUTPUT);
  ledcSetup(PWM_SPEAKER_CHANNEL, PWM_SPEAKER_FREQUENCY, PWM_SPEAKER_RESOLUTION);
  ledcAttachPin(PIN_SPEAKER, PWM_SPEAKER_CHANNEL);
  pinMode(PIN_AMPLIFIER, OUTPUT);
  digitalWrite(PIN_AMPLIFIER, HIGH);

  parts.listOfSounds[SOUND_POWER_UP] = soundPowerUp;
  parts.listOfLengthOfSounds[SOUND_POWER_UP] = sizeof(soundPowerUp)/sizeof(soundPowerUp[0]);
  parts.listOfSounds[SOUND_POWER_DOWN] = soundPowerDown;
  parts.listOfLengthOfSounds[SOUND_POWER_DOWN] = sizeof(soundPowerDown)/sizeof(soundPowerDown[0]);
  parts.listOfSounds[SOUND_KEY_INSERT] = soundKeyInsert;
  parts.listOfLengthOfSounds[SOUND_KEY_INSERT] = sizeof(soundKeyInsert)/sizeof(soundKeyInsert[0]);

  setupLaserGrid();
  lgComponents.state = SETUP;
}

void loop()
{
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );

  // Map puzzle's values with component's values
  LaserGrid::update(puzzle, lgComponents);

  // State changes
  LaserGrid::run(lgComponents);

  // Show changes
  LaserGrid::show(lgComponents);
}

void setupLaserGrid()
{
  lgComponents.waveAdjuster.set(PIN_ANALOG_INPUT_1, PIN_ANALOG_INPUT_2, PIN_ANALOG_INPUT_3, &Serial2);
  lgComponents.keyReader.set(PIN_INPUT_1, PIN_INPUT_2, PIN_INPUT_3, PIN_RELAY_1);
  lgComponents.powerSwitch.set(parts.strip, PIN_LIGHT_FOR_POWER_SWITCH, PIN_SWITCH_1);
  lgComponents.speaker.set(PIN_SPEAKER, PIN_AMPLIFIER, 65, parts.listOfSounds, parts.listOfLengthOfSounds, PWM_SPEAKER_CHANNEL);
}
