#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include <Adafruit_MCP23017.h>
#include <ESP32Encoder.h>
#include "lib/SevenSegment.h"
#include "lib/Datamatic.h"
#include "sounds/soundPowerUp.h"
//#include "sounds/soundPowerDown.h"
Puzzle puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  Adafruit_MCP23017 mcp1;
  Adafruit_MCP23017 mcp2;
  ESP32Encoder encoder;
  SevenSegment matrix = SevenSegment();
  unsigned char* listOfSounds[NUMBER_OF_SOUNDS];
  unsigned int listOfLengthOfSounds[NUMBER_OF_SOUNDS];
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

Datamatic::Components dmComponents;

void setupDatamatic();

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
  
  // Setup 7 segment LED
  parts.matrix.begin(ADDR_SEVENSEGMENT);
  parts.matrix.setNumberOfDigits(5);
  
  // Setup power switch
  pinMode(PIN_SWITCH_1, INPUT);
  pinMode(PIN_SWITCH_2, INPUT);
  pinMode(PIN_SWITCH_3, INPUT);

  // Setup input switch
  pinMode(PIN_INPUT_1, INPUT);
  pinMode(PIN_INPUT_2, INPUT);
  pinMode(PIN_INPUT_3, INPUT);

  // Setup pins for I2C buttons
  parts.mcp1.begin(ADDR_BUTTON_1);
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS_1; i++) {
    parts.mcp1.pinMode(buttonPins1[i], INPUT);
  }
  parts.mcp2.begin(ADDR_BUTTON_2);
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS_2; i++) {
    parts.mcp2.pinMode(buttonPins2[i], INPUT);
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
  
  setupDatamatic();
  dmComponents.state = SETUP;
}

void loop() 
{
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );
  
  Datamatic::update(puzzle, dmComponents);

  // Enable Datamatic
  Datamatic::run(dmComponents);

  Datamatic::show(dmComponents);
}

void setupDatamatic()
{
  dmComponents.informationDisplay.set(&parts.encoder);
  dmComponents.codeReader.set(&parts.matrix, &parts.mcp1, buttonPins1, &parts.mcp2, buttonPins2, buttonLabels, PIN_INPUT_1, PIN_INPUT_2);
  dmComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitch, PIN_SWITCH_1);
  dmComponents.lightEffect.set(parts.strip, lightPinsForLightEffect);
  dmComponents.speaker.set(PIN_SPEAKER, PIN_AMPLIFIER, 65, parts.listOfSounds, parts.listOfLengthOfSounds);
}
