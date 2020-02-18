#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <ESP32Encoder.h>
#include "lib/PowerPanel.h"
#include "sounds/soundPowerUp.h"
// #include "sounds/soundPowerDown.h"

Puzzle puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  ESP32Encoder encoder;
  Adafruit_7segment matrix1 = Adafruit_7segment(); 
  Adafruit_7segment matrix2 = Adafruit_7segment();
  unsigned char* listOfSounds[NUMBER_OF_SOUNDS];
  unsigned int listOfLengthOfSounds[NUMBER_OF_SOUNDS];
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

PowerPanel::Components ppComponents;

void setupPowerPanel();

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
  parts.matrix1.begin(ADDR_SEVENSEGMENT_1);
  parts.matrix2.begin(ADDR_SEVENSEGMENT_2);
  
  // Setup power switch
  pinMode(PIN_SWITCH_1, INPUT);
  pinMode(PIN_SWITCH_2, INPUT);
  pinMode(PIN_SWITCH_3, INPUT);
  
  // configure PWM functionalitites for Analog Voltage Gauge
  /*
   * channel = 0 or 1 or 2 or 3
   * frequency = 5000
   * resolution = 8 or 10 or 12 or 15
   * PWM value = from 0 (0%) to 1024 (100%)
   */
  pinMode(PIN_OUTPUT_1, OUTPUT);
  ledcSetup(PWM_OUTPUT_1_CHANNEL, PWM_OUTPUT_1_FREQUENCY, PWM_OUTPUT_1_RESOLUTION);
  ledcAttachPin(PIN_OUTPUT_1, PWM_OUTPUT_1_CHANNEL);

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

  setupPowerPanel();
  ppComponents.state = SETUP;
  puzzle.startTime = millis();
}

void loop() 
{ 
  // Save timer to the variable
  puzzle.timer = millis();
  // Enable communication to master
  parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );
  
  // Map puzzle's values with component's values
  PowerPanel::update(puzzle, ppComponents);

  // Enable Power Panel
  PowerPanel::run(ppComponents);

  PowerPanel::show(ppComponents);
}

void setupPowerPanel()
{
  ppComponents.powerAdjuster.set(&parts.encoder, &parts.matrix1, &parts.matrix2, PWM_OUTPUT_1_CHANNEL);
  ppComponents.powerLightIndicator.set(parts.strip, lightPinForPowerLightIndicator);
  ppComponents.battery.set(parts.strip, lightPinsForBarIndicator);
  ppComponents.lightEffect.set(parts.strip, lightPinsForLightEffect);
  ppComponents.speaker.set(PIN_SPEAKER, PIN_AMPLIFIER, 65, parts.listOfSounds, parts.listOfLengthOfSounds);
}
