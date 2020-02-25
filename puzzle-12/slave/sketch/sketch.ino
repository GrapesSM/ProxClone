#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "lib/BlastDoorKeyPad.h"
#include <ESP32Encoder.h>
#include "sounds/soundPowerUp.h"
//#include "sounds/soundPowerDown.h"


Puzzle puzzle;

struct Parts {
  Modbus * slave;
  KEYPAD keypad;
  ESP32Encoder encoder;
  unsigned char* listOfSounds[NUMBER_OF_SOUNDS];
  unsigned int listOfLengthOfSounds[NUMBER_OF_SOUNDS];
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);

BlastDoorKeypad::Components bdComponents;

void setupBlastDoorKeypad();

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);

  // Setup Modbus communication
  parts.slave = &slave;
  parts.slave->begin( SERIAL_BAUDRATE, PIN_RX_1, PIN_TX_1 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  //Setup and Init Encoders
  ESP32Encoder::useInternalWeakPullResistors=false;
  //-- adjust starting count value to 0
  parts.encoder.clearCount();
  parts.encoder.setCount(0);
  //-- attach pins for use as encoder pins
  parts.encoder.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);

  //Keypad setup
  
  if (parts.keypad.begin() == false) 	// Note, using begin() like this will use default I2C address, 0x4B. 
                // You can pass begin() a different address like so: keypad1.begin(Wire, 0x4A).
  {
    Serial.println("Keypad does not appear to be connected. Please check wiring. Freezing...");
    while (1);
  }

  // Setup speaker pins
  pinMode(PIN_SPEAKER, OUTPUT);
  ledcSetup(PWM_SPEAKER_CHANNEL, PWM_SPEAKER_FREQUENCY, PWM_SPEAKER_RESOLUTION);
  ledcAttachPin(PIN_SPEAKER, PWM_SPEAKER_CHANNEL);
  pinMode(PIN_AMPLIFIER, OUTPUT);
  digitalWrite(PIN_AMPLIFIER, HIGH);

  parts.listOfSounds[SOUND_POWER_UP] = soundPowerUp;
  parts.listOfLengthOfSounds[SOUND_POWER_UP] = sizeof(soundPowerUp)/sizeof(soundPowerUp[0]);
//  parts.listOfSounds[SOUND_POWER_DOWN] = soundPowerDown;
//  parts.listOfLengthOfSounds[SOUND_POWER_DOWN] = sizeof(soundPowerDown)/sizeof(soundPowerDown[0]);

  
  setupBlastDoorKeypad();
  bdComponents.state = SETUP;
}

void loop() 
{
  puzzle.timer = millis();
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );
  
  // Map puzzle's values to component's values
  BlastDoorKeypad::update(puzzle, bdComponents);

  // State changes
  BlastDoorKeypad::run(bdComponents);
  
  // Show changes
  BlastDoorKeypad::show(bdComponents);
}

void setupBlastDoorKeypad()
{
  bdComponents.codeReader.set(&parts.keypad, PIN_INPUT_1);
  bdComponents.speaker.set(PIN_SPEAKER, PIN_AMPLIFIER, 65, parts.listOfSounds, parts.listOfLengthOfSounds);
}
