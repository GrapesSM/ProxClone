#include "Constants.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include <Wire.h>
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
void runTaskFunction(void*);
void showTaskFunction(void*);

TaskHandle_t runTask;
TaskHandle_t showTask;

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial);
  
  // Setup 7 segment LED
  parts.matrix.begin(ADDR_SEVENSEGMENT);

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
    // Setup Task functions

  int temp1 = xTaskCreatePinnedToCore(
    runTaskFunction,   /* Task function. */
    "RunTask",     /* name of task. */
    100000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &runTask,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */                  
  
  if(temp1) {
    Serial.println("Task created...");
  } else {
    Serial.printf("Couldn't create task %i", temp1);
  }
  
  int temp2 = xTaskCreatePinnedToCore(
    showTaskFunction,   /* Task function. */
    "ShowTask",     /* name of task. */
    60000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &showTask,      /* Task handle to keep track of created task */
    1);

  if(temp2) {
    Serial.println("Task created...");
  } else {
    Serial.printf("Couldn't create task %i", temp2);
  }
    
  lsComponents.state = SETUP;
}

void loop() { vTaskDelete(NULL); }
//void loop() { vTaskDelay(100); }

void setupLifeSupport()
{
  lsComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitch, PIN_SWITCH_1);
  lsComponents.externalVent.set(parts.strip, lightPinsForExternalVent, PIN_INPUT_1);
  lsComponents.airSupplyPump.set(parts.strip, lightPinsForAirSupplyPump, PIN_INPUT_2);
  lsComponents.airPressureStatus.set(parts.strip, lightPinsForAirPressureStatus, &parts.matrix);
  lsComponents.speaker.set(PIN_SPEAKER, PIN_AMPLIFIER, 65, parts.listOfSounds, parts.listOfLengthOfSounds, PWM_SPEAKER_CHANNEL);
  lsComponents.lightEffect.set(parts.strip, lightPinsForLightEffect);
}

//Run Task Function: process changes of puzzle
void runTaskFunction( void * parameters ) {
  Serial.print("Run Task running on core ");
  Serial.println(xPortGetCoreID());
  
  for(;;){

    // Map puzzle's values to component's values
    LifeSupport::update(puzzle, lsComponents);

    // State changes
    LifeSupport::run(lsComponents);
    
    // Show changes
    LifeSupport::show(lsComponents);

    vTaskDelay(10);
  } 
}

//Show Task Fucntion: shows changes of puzzle
void showTaskFunction( void * parameters ){
  Serial.print("Show Task running on core ");
  Serial.println(xPortGetCoreID());
  
  for(;;){
    // Enable communication to master
    parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );
  } 
}
