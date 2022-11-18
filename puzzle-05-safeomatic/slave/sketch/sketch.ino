#include "Constants.h"
#include "SPIFFS.h"
#include "lib/ModbusRtu.h"
#include "NeoPixelBus.h"
#include <ESP32Encoder.h>
#include "lib/Safeomatic.h"

Puzzle puzzle;

struct Parts {
  Modbus * slave;
  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> * strip;
  ESP32Encoder encoder;
} parts;

Modbus slave(puzzle.address, 1, PIN_485_EN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(LED_COUNT, PIN_NEOPIXEL);

Safeomatic::Components smComponents;

void setupSafeomatic();
void runTaskFunction(void*);
void showTaskFunction(void*);

TaskHandle_t runTask;
TaskHandle_t showTask;

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial);

  SPIFFS.begin();

  // Setup Modbus communication
  parts.slave = &slave;
  parts.slave->begin( SERIAL_BAUDRATE, PIN_RX_2, PIN_TX_2 );
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  // Setup NeoPixelBus for all lights
  parts.strip = &strip;
  parts.strip->Begin();
  parts.strip->Show();

  // Setup and Init Encoders
  ESP32Encoder::useInternalWeakPullResistors=DOWN;
  //-- attach pins for use as encoder pins
  parts.encoder.attachHalfQuad(PIN_ENCODER_A, PIN_ENCODER_B);
  //-- adjust starting count value to 0
  parts.encoder.clearCount();
  parts.encoder.setCount(0);
   
  // Setup power switch
  pinMode(PIN_INPUT_1, INPUT);
  pinMode(PIN_RELAY_1, OUTPUT);
  digitalWrite(PIN_RELAY_1, LOW);
  pinMode(PIN_RELAY_2, OUTPUT);
  digitalWrite(PIN_RELAY_2, LOW);

  // Setup speaker pins
  pinMode(PIN_SPEAKER, OUTPUT);
  pinMode(PIN_AMPLIFIER, OUTPUT);
  digitalWrite(PIN_AMPLIFIER, HIGH);

  setupSafeomatic();

  // Setup Task functions
  xTaskCreatePinnedToCore(
    runTaskFunction,   /* Task function. */
    "RunTask",     /* name of task. */
    90000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &runTask,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */                  

//  delay(500);

  xTaskCreatePinnedToCore(
    showTaskFunction,   /* Task function. */
    "ShowTask",     /* name of task. */
    90000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &showTask,      /* Task handle to keep track of created task */
    1);
  smComponents.state = SETUP;
}

void loop()
{

}

void setupSafeomatic()
{
  smComponents.combinationReader.set(parts.strip, lightPinsForCombinationReader, &parts.encoder);
  smComponents.powerSwitch.set(parts.strip, lightPinForPowerSwitch, PIN_SWITCH_1);
  smComponents.accessPanel.set(PIN_INPUT_1, PIN_RELAY_2);
  smComponents.door.set(parts.strip, lightPinForSafe, PIN_RELAY_1);
  smComponents.speaker.set(soundFilenames);
}

//Run Task Function: process changes of puzzle
void runTaskFunction( void * parameters ) {
  Serial.print("Run Task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    // Map puzzle's values to component's values
    Safeomatic::update(puzzle, smComponents);

    // State changes
    Safeomatic::run(smComponents);

    // Show changes
    Safeomatic::show(smComponents);

    // Produce sounds
    Safeomatic::sound(smComponents);
    
    vTaskDelay(10);
  } 
}

//Show Task Function: shows changes of puzzle
void showTaskFunction( void * parameters ){
  Serial.print("Show Task running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    // Enable communication to master
//    if (Serial1.available() > 0)
//    {
    parts.slave->poll( puzzle.registers, puzzle.numberOfRegisters );
//    }
  } 
}
