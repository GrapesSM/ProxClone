#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include "lib/PrepStatus.h"
#include "sounds/soundPowerUp.h"
//#include "sounds/soundPowerDown.h"
//#include "sounds/soundKeyInsert.h"

Puzzle puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  unsigned char* listOfSounds[NUMBER_OF_SOUNDS];
  unsigned int listOfLengthOfSounds[NUMBER_OF_SOUNDS];
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
  
  setupPrepStatus();
  psComponents.state = SETUP;
}

void loop()
{
  puzzle.timer = millis();  
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );

  for (int i = 0; i < puzzle.numberOfRegisters; i++) {
    Serial.print(puzzle.registers[i]);
    if (i < puzzle.numberOfRegisters - 1) Serial.print(",");
    else Serial.println();
  }

  PrepStatus::update(puzzle, psComponents);

  // Enable Energy Supplemental
  PrepStatus::run(psComponents);

  PrepStatus::show(psComponents);
}

void setupPrepStatus()
{
  psComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitch, PIN_SWITCH_1);
  psComponents.batteryMatrix.set(parts.strip, lightPinsForBatteryMatrix);
  psComponents.energySupp.set(parts.strip, lightPinsForEnergySupp);
  psComponents.generator.set(parts.strip, lightPinsForGenerator);
  psComponents.syncroReader.set(parts.strip, lightPinsForSyncroReader, PIN_INPUT_1);  
  psComponents.lightEffect.set(parts.strip, lightPinsForLightEffect);
  psComponents.speaker.set(PIN_SPEAKER, PIN_AMPLIFIER, 65, parts.listOfSounds, parts.listOfLengthOfSounds);
}
