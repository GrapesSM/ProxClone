#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "lib/LifeSupport.h"
#include "sounds/soundPowerUp.h"
//#include "sounds/soundPowerDown.h"
//#include "sounds/soundKeyInsert.h"

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

  // Setup speaker pins
  pinMode(PIN_SPEAKER, OUTPUT);
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(PIN_SPEAKER, PWM_CHANNEL);
  pinMode(PIN_AMPLIFIER, OUTPUT);
  digitalWrite(PIN_AMPLIFIER, HIGH);

  // Setup sound list
  parts.listOfSounds[SOUND_POWER_UP] = soundPowerUp;
  parts.listOfLengthOfSounds[SOUND_POWER_UP] = sizeof(soundPowerUp)/sizeof(soundPowerUp[0]);
  // parts.listOfSounds[SOUND_POWER_DOWN] = soundPowerDown;
  // parts.listOfLengthOfSounds[SOUND_POWER_DOWN] = sizeof(soundPowerDown)/sizeof(soundPowerDown[0]);
  // parts.listOfSounds[SOUND_KEY_INSERT] = soundKeyInsert;
  // parts.listOfLengthOfSounds[SOUND_KEY_INSERT] = sizeof(soundKeyInsert)/sizeof(soundKeyInsert[0]);

  setupLifeSupport();

  puzzle.timer = millis();
}

void loop() 
{
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );

  // Enable Energy Supplemental
  LifeSupport::run(lsComponents);

  puzzle.timer = millis();
  if (puzzle.timer - puzzle.checkpoint > puzzle.interval) {
    puzzle.checkpoint = millis();
    LifeSupport::show(lsComponents);
  }
}

void setupLifeSupport()
{
  lsComponents.externalVent.set();
  lsComponents.airSupplyPump.set();
  lsComponents.airPressureStatus.set();
  lsComponents.powerSwitch.set();
  lsComponents.speaker.set(PIN_SPEAKER, PIN_AMPLIFIER, 65, parts.listOfSounds, parts.listOfLengthOfSounds);
  lsComponents.lightEffect.set();
}
