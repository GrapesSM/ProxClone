#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "lib/LifeSupport.h"
#include "sounds/soundPowerUp.h"
#include "sounds/soundPowerDown.h"

Puzzle puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  Adafruit_7segment matrix = Adafruit_7segment(); 
  unsigned char* listOfSounds[NUMBER_OF_SOUNDS];
  unsigned int listOfLengthOfSounds[NUMBER_OF_SOUNDS];
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

LifeSupport::Components lsComponents;

void setupLifeSupport();

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
  
  // Setup power switch
  pinMode(PIN_SWITCH_1, INPUT);
  pinMode(PIN_SWITCH_2, INPUT);
  pinMode(PIN_SWITCH_3, INPUT);

  // Setup power switch
  pinMode(PIN_INPUT_1, INPUT);
  pinMode(PIN_INPUT_2, INPUT);

  // Setup speaker pins
  pinMode(PIN_SPEAKER, OUTPUT);
  ledcSetup(PWM_SPEAKER_CHANNEL, PWM_SPEAKER_FREQUENCY, PWM_SPEAKER_RESOLUTION);
  ledcAttachPin(PIN_SPEAKER, PWM_SPEAKER_CHANNEL);
  pinMode(PIN_AMPLIFIER, OUTPUT);
  digitalWrite(PIN_AMPLIFIER, HIGH);

  // Setup sound list
  parts.listOfSounds[SOUND_POWER_UP] = soundPowerUp;
  parts.listOfLengthOfSounds[SOUND_POWER_UP] = sizeof(soundPowerUp)/sizeof(soundPowerUp[0]);
  parts.listOfSounds[SOUND_POWER_DOWN] = soundPowerDown;
  parts.listOfLengthOfSounds[SOUND_POWER_DOWN] = sizeof(soundPowerDown)/sizeof(soundPowerDown[0]);

  setupLifeSupport();
  lsComponents.state = SETUP;
}

void loop() 
{
  // Save timings
  puzzle.timer = millis();
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );

  // Map puzzle's values to component's values
  LifeSupport::update(puzzle, lsComponents);

  // Process change and assign states
  LifeSupport::run(lsComponents);

  // Show changes on the panel
  LifeSupport::show(lsComponents);
}

void setupLifeSupport()
{
  lsComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitch, PIN_SWITCH_1);
  lsComponents.externalVent.set(parts.strip, lightPinsForExternalVent, PIN_INPUT_1);
  lsComponents.airSupplyPump.set(parts.strip, lightPinsForAirSupplyPump, PIN_INPUT_2);
  lsComponents.airPressureStatus.set(parts.strip, lightPinsForAirPressureStatus, &parts.matrix);
  lsComponents.speaker.set(PIN_SPEAKER, PIN_AMPLIFIER, 65, parts.listOfSounds, parts.listOfLengthOfSounds, PWM_SPEAKER_CHANNEL);
  lsComponents.lightEffect.set(parts.strip, lightPinsForLightEffect);
}
